#include "precompiled.h"
#include "stream_object.h"

#include "syntax/files/file.h"
#include "syntax/filters/filter.h"
#include "syntax/utils/name_constants.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/streams/stream_utils.h"

namespace vanillapdf {
namespace syntax {

StreamObject::StreamObject() : StreamObject(false) {
    _access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

StreamObject::StreamObject(bool initialized) {
    _header->SetOwner(Object::GetWeakReference());

    _access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());

    SetInitialized(initialized);
}

StreamObject::StreamObject(DictionaryObjectPtr header, types::stream_offset offset)
    : _header(header), _raw_data_offset(offset) {
    _header->SetOwner(Object::GetWeakReference());

    _access_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

bool StreamObject::IsDirty() const {
    ACCESS_LOCK_GUARD(_access_lock);

    if (m_version > 0) return true;
    if (_header->IsDirty()) return true;

    // Only check the decoded body buffer (user-facing layer).
    // Raw and decrypted buffers are internal lazy-loading caches
    // that should not be modified directly by users.
    if (_body_decoded->GetVersion() > 0) return true;
    return false;
}

DictionaryObjectPtr StreamObject::GetHeader() const {
    return _header;
}

void StreamObject::SetHeader(DictionaryObjectPtr header) {
    ACCESS_LOCK_GUARD(_access_lock);

    _header = header;
    IncrementVersion();
}

void StreamObject::SetBody(BufferPtr value) {
    ACCESS_LOCK_GUARD(_access_lock);

    _body_decoded->assign(value.begin(), value.end());
    _body_decoded->SetInitialized();

    IncrementVersion();
}

types::stream_offset StreamObject::GetDataOffset() const {
    return _raw_data_offset;
}

void StreamObject::SetDataOffset(types::stream_offset offset) {
    ACCESS_LOCK_GUARD(_access_lock);

    _raw_data_offset = offset;
    IncrementVersion();
}

Object::Type StreamObject::GetObjectType(void) const noexcept {
    return Object::Type::Stream;
}

StreamObject* StreamObject::Clone(void) const {
    StreamObjectPtr result(pdf_new StreamObject(), false);

    ACCESS_LOCK_GUARD(_access_lock);

    result->_header = _header->Clone();
    result->_header->SetInitialized();

    result->_raw_data_offset = _raw_data_offset;

    if (_body_raw->IsInitialized()) {
        result->_body_raw = _body_raw->Clone();
        result->_body_raw->SetInitialized();
    }

    if (_body_decrypted->IsInitialized()) {
        result->_body_decrypted = _body_decrypted->Clone();
        result->_body_decrypted->SetInitialized();
    }

    if (_body_decoded->IsInitialized()) {
        result->_body_decoded = _body_decoded->Clone();
        result->_body_decoded->SetInitialized();
    }

    result->SetInitialized();

    CloneBaseProperties(result);
    return result.detach();
}

void StreamObject::SetFile(WeakReference<File> file) {
    ACCESS_LOCK_GUARD(_access_lock);

    Object::SetFile(file);
    _header->SetFile(file);
}

void StreamObject::SetInitialized(bool initialized) {
    ACCESS_LOCK_GUARD(_access_lock);

    Versionable::SetInitialized(initialized);
    _header->SetInitialized(initialized);

    // In case the object is already initialized without data offset
    // consider it data to be initialized as well
    if (initialized && _raw_data_offset == constant::BAD_OFFSET) {
        _body_raw->SetInitialized();
    }
}

std::optional<types::stream_offset> StreamObject::CaptureBodyOffset() const {
    ACCESS_LOCK_GUARD(_access_lock);

    if (_body_raw->IsInitialized()) {
        return std::nullopt;
    }

    if (!m_file.IsActive()) {
        throw FileDisposedException();
    }

    if (_raw_data_offset == constant::BAD_OFFSET) {
        throw ParseException("Stream object data offset is not initialized");
    }

    return _raw_data_offset;
}

void StreamObject::LoadBody(types::stream_offset offset) const {
    // _access_lock (M_stream) is intentionally NOT held here.
    // GetInputStream() acquires the InputStream lock (M0) internally,
    // and holding M_stream across that call creates a lock-order inversion with parsing
    // (which holds M0 while calling SetFile() -> acquiring M_stream on new objects).
    if (!m_file.IsActive()) {
        throw FileDisposedException();
    }

    auto locked_file = m_file.GetReference();
    auto input = locked_file->GetInputStream();

    input->ExclusiveInputLock();
    input->SetInputPosition(offset);

    auto pos = input->GetInputPosition();

    auto cleanup_lambda = [&input, pos]() {
        input->SetInputPosition(pos);
        input->ExclusiveInputUnlock();
    };

    SCOPE_GUARD(cleanup_lambda);

    auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
    auto body = input->Read(size->SafeConvert<types::size_type>());

    // Re-acquire M_stream to cache; skip if another thread already loaded it.
    ACCESS_LOCK_GUARD(_access_lock);
    if (!_body_raw->IsInitialized()) {
        _body_raw->assign(body.begin(), body.end());
        _body_raw->SetInitialized();
    }
}

BufferPtr StreamObject::GetBodyRaw() const {

    if (_body_raw->IsInitialized()) {
        return _body_raw;
    }

    // Two-phase approach to prevent the M_stream -> M0 lock-order inversion that TSan reports.
    // Phase 1 — CaptureBodyOffset(): acquire M_stream, read the data offset, release M_stream.
    // Phase 2 — LoadBody(): call GetInputStream() WITHOUT M_stream, so M0
    //   (InputStream) can be acquired without creating the cycle:
    //   M0 (parsing) -> M_stream (SetFile) -> M0 (GetInputStream).
    auto offset = CaptureBodyOffset();
    if (!offset) {
        // Another thread loaded the body between the fast-path check
        // and the lock acquisition inside CaptureBodyOffset().
        return _body_raw;
    }

    LoadBody(*offset);
    return _body_raw;
}

BufferPtr StreamObject::GetBody() const {

    if (_body_decoded->IsInitialized()) {
        return _body_decoded;
    }

    // Pre-load raw body without _access_lock to avoid M_stream -> M0 inversion.
    // See GetBodyRaw() for the lock-ordering rationale.
    GetBodyRaw();

    ACCESS_LOCK_GUARD(_access_lock);

    if (_body_decoded->IsInitialized()) {
        return _body_decoded;
    }

    if (!_header->Contains(constant::Name::Filter)) {
        auto body = GetBodyDecrypted();

        _body_decoded->assign(body.begin(), body.end());
        _body_decoded->SetInitialized();

        return _body_decoded;
    }

    //auto stream = locked_file->GetInputStream();
    //auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
    //auto pos = stream->tellg();
    //stream->seekg(_raw_data_offset);
    //SCOPE_GUARD_CAPTURE_VALUES(stream->seekg(pos));

    auto filter_obj = _header->Find(constant::Name::Filter);
    bool is_filter_null = ObjectUtils::IsType<NullObjectPtr>(filter_obj);
    bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
    bool is_filter_array = ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

    if (is_filter_null) {
        return GetBodyDecrypted();
    }

    if (is_filter_name) {
        auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
        if (filter_name == constant::Name::Crypt) {
            auto body = GetBodyDecrypted();
            _body_decoded->assign(body.begin(), body.end());
            _body_decoded->SetInitialized();
            return _body_decoded;
        }

        auto filter = FilterBase::GetFilterByName(filter_name);
        if (_header->Contains(constant::Name::DecodeParms)) {
            auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
            auto body_decrypted = GetBodyDecrypted();
            auto body = filter->Decode(body_decrypted, params, m_attributes);
            _body_decoded->assign(body.begin(), body.end());
            _body_decoded->SetInitialized();
            return _body_decoded;
        }

        auto body_decrypted = GetBodyDecrypted();
        auto body = filter->Decode(body_decrypted, DictionaryObjectPtr(), m_attributes);
        _body_decoded->assign(body.begin(), body.end());
        _body_decoded->SetInitialized();
        return _body_decoded;
    }

    if (is_filter_array) {
        auto filter_array = _header->FindAs<ArrayObjectPtr<NameObjectPtr>>(constant::Name::Filter);

        MixedArrayObjectPtr params;
        bool has_params = _header->Contains(constant::Name::DecodeParms);
        if (has_params) {
            params = _header->FindAs<MixedArrayObjectPtr>(constant::Name::DecodeParms);
            assert(filter_array->GetSize() == params->GetSize());
        }

        BufferPtr result = GetBodyDecrypted();
        for (unsigned int i = 0; i < filter_array->GetSize(); ++i) {
            auto current_filter = (*filter_array)[i];
            if (current_filter == constant::Name::Crypt) {
                continue;
            }

            auto filter = FilterBase::GetFilterByName(current_filter);

            if (has_params) {
                auto current_param = (*params)[i];
                bool is_param_null = ObjectUtils::IsType<NullObjectPtr>(current_param);
                if (is_param_null) {
                    result = filter->Decode(result, DictionaryObjectPtr(), m_attributes);
                    continue;
                }

                auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(current_param);
                result = filter->Decode(result, dict, m_attributes);
                continue;
            }

            result = filter->Decode(result, DictionaryObjectPtr(), m_attributes);
        }

        _body_decoded->assign(result.begin(), result.end());
        _body_decoded->SetInitialized();
        return _body_decoded;
    }

    assert(is_filter_name ^ is_filter_array);
    throw ParseException("Filter is neither name nor array of names");
}

BufferPtr StreamObject::GetBodyEncoded() const {

    auto locked_file = m_file.GetReference();

    bool is_file_encrypted = locked_file->IsEncrypted();
    bool is_file_encryption_dirty = locked_file->IsEncryptionDirty();

    // Optimization for unchanged streams
    // In case the document is encrypted, the stream contents needs to be recalculated
    // Encryption dirty flag is toggled in case there were changes to the encryption at the file level

    if (!is_file_encrypted && !is_file_encryption_dirty) {
        bool is_dirty = IsDirty();
        bool is_body_initialized = _body_decoded->IsInitialized();

        // In case the object is not dirty, we can safely return the original stream data

        if (!is_dirty) {
            return GetBodyRaw();
        }

        // We need to avoid cases, where there is non-significant change in the stream header
        // as we are always losing some kind of precision, when re-encoding images.
        // It's better to let user handle the scenario of changing stream contents explicitly.

        if (!is_body_initialized) {
            return GetBodyRaw();
        }
    }

    // Extended optimization that works even if the original file was encrypted.
    // This is especially important for add/remove encryption, as the images were always distorted.
    // We are not actually changing the images, so let's keep the content intact.

    if (!IsDirty()) {
        auto body_decrypted = GetBodyDecrypted();
        return EncryptStream(body_decrypted, GetRootObjectNumber(), GetRootGenerationNumber());
    }

    auto decoded_body = GetBody();

    if (!_header->Contains(constant::Name::Filter)) {
        return EncryptStream(decoded_body, GetRootObjectNumber(), GetRootGenerationNumber());
    }

    auto filter_obj = _header->Find(constant::Name::Filter);
    bool is_filter_null = ObjectUtils::IsType<NullObjectPtr>(filter_obj);
    bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
    bool is_filter_array = ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

    if (is_filter_null) {
        return EncryptStream(decoded_body, GetRootObjectNumber(), GetRootGenerationNumber());
    }

    if (is_filter_name) {
        auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
        if (filter_name == constant::Name::Crypt) {
            auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
            auto handler_name = params->FindAs<NameObjectPtr>(constant::Name::Name);
            return EncryptData(decoded_body, GetRootObjectNumber(), GetRootGenerationNumber(), handler_name);
        }

        auto filter = FilterBase::GetFilterByName(filter_name);
        if (_header->Contains(constant::Name::DecodeParms)) {
            auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
            auto result = filter->Encode(decoded_body, params, m_attributes);
            return EncryptStream(result, GetRootObjectNumber(), GetRootGenerationNumber());
        }

        auto result = filter->Encode(decoded_body, DictionaryObjectPtr(), m_attributes);
        return EncryptStream(result, GetRootObjectNumber(), GetRootGenerationNumber());
    }

    if (is_filter_array) {
        auto filter_array = _header->FindAs<ArrayObjectPtr<NameObjectPtr>>(constant::Name::Filter);

        MixedArrayObjectPtr params;
        bool has_params = _header->Contains(constant::Name::DecodeParms);
        if (has_params) {
            params = _header->FindAs<MixedArrayObjectPtr>(constant::Name::DecodeParms);

            auto filters_size = filter_array->GetSize();
            auto params_size = params->GetSize();

            assert(filters_size == params_size && "Filter size does not correspond to the params size");
            UNUSED(filters_size); UNUSED(params_size);
        }

        auto filters_size = filter_array->GetSize();
        for (decltype(filters_size) i = 0; i < filters_size; ++i) {
            auto current_filter = (*filter_array)[i];
            if (current_filter == constant::Name::Crypt) {
                decoded_body = EncryptStream(decoded_body, GetRootObjectNumber(), GetRootGenerationNumber());
            }

            auto filter = FilterBase::GetFilterByName(current_filter);

            if (has_params && i < params->GetSize()) {
                auto current_param = params->GetValue(i);
                bool is_param_dictionary = ObjectUtils::IsType<DictionaryObjectPtr>(current_param);
                if (is_param_dictionary) {
                    auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(current_param);
                    decoded_body = filter->Encode(decoded_body, dict, m_attributes);
                    continue;
                }

                bool is_param_null = ObjectUtils::IsType<NullObjectPtr>(current_param);

                assert(is_param_null && "Unknown filter parameter type");
                UNUSED(is_param_null);
            }

            decoded_body = filter->Encode(decoded_body, DictionaryObjectPtr(), m_attributes);
        }

        return decoded_body;
    }

    assert(is_filter_name ^ is_filter_array);
    throw ParseException("Filter is neither name nor array of names");
}

BufferPtr StreamObject::GetBodyDecrypted() const {

    if (_body_decrypted->IsInitialized()) {
        return _body_decrypted;
    }

    // Pre-load raw body without _access_lock to avoid M_stream -> M0 inversion.
    // See GetBodyRaw() for the lock-ordering rationale.
    auto body_raw = GetBodyRaw();

    ACCESS_LOCK_GUARD(_access_lock);

    if (_body_decrypted->IsInitialized()) {
        return _body_decrypted;
    }

    auto locked_file = m_file.GetReference();

    // During the initialization it is unknown whether a file is encrypted
    // This is important for object streams that are being parsed before encryption dictionary
    bool is_file_encrypted = locked_file->IsInitialized() && locked_file->IsEncrypted();

    if (IsEncryptionExempted() || !is_file_encrypted) {
        _body_decrypted->assign(body_raw.begin(), body_raw.end());
        _body_decrypted->SetInitialized();
        return _body_decrypted;
    }

    // Stream does not contain crypt filter
    if (!_header->Contains(constant::Name::Filter)) {
        auto result = locked_file->DecryptStream(body_raw, GetRootObjectNumber(), GetRootGenerationNumber());

        _body_decrypted->assign(result.begin(), result.end());
        _body_decrypted->SetInitialized();
        return _body_decrypted;
    }

    auto filter_obj = _header->Find(constant::Name::Filter);
    bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
    bool is_filter_array = ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

    if (is_filter_name) {
        auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
        if (filter_name == constant::Name::Crypt) {
            auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
            auto handler_name = params->FindAs<NameObjectPtr>(constant::Name::Name);
            auto result = locked_file->DecryptData(body_raw, GetRootObjectNumber(), GetRootGenerationNumber(), handler_name);

            _body_decrypted->assign(result.begin(), result.end());
            _body_decrypted->SetInitialized();
            return _body_decrypted;
        }
    }

    if (is_filter_array) {
        auto filter_array = _header->FindAs<ArrayObjectPtr<NameObjectPtr>>(constant::Name::Filter);
        for (unsigned int i = 0; i < filter_array->GetSize(); ++i) {
            auto current_filter = (*filter_array)[i];
            if (current_filter == constant::Name::Crypt) {
                assert(i == 0 && "Crypt filter is not first");
                auto params = _header->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::DecodeParms);
                auto handler_name = params->GetValue(i)->FindAs<NameObjectPtr>(constant::Name::Name);
                auto result = locked_file->DecryptData(body_raw, GetRootObjectNumber(), GetRootGenerationNumber(), handler_name);

                _body_decrypted->assign(result.begin(), result.end());
                _body_decrypted->SetInitialized();
                return _body_decrypted;
            }
        }
    }

    // Stream does not contain crypt filter
    auto result = locked_file->DecryptStream(body_raw, GetRootObjectNumber(), GetRootGenerationNumber());

    _body_decrypted->assign(result.begin(), result.end());
    _body_decrypted->SetInitialized();
    return _body_decrypted;
}

BufferPtr StreamObject::EncryptStream(BufferPtr data, types::big_uint obj_number, types::ushort generation_number) const {

    auto locked_file = m_file.GetReference();

    // During the initialization it is unknown whether a file is encrypted
    // This is important for object streams that are being parsed before encryption dictionary
    bool is_file_encrypted = locked_file->IsInitialized() && locked_file->IsEncrypted();

    if (IsEncryptionExempted() || !is_file_encrypted) {
        return data;
    }

    auto result = locked_file->EncryptStream(data, obj_number, generation_number);

#ifdef DEBUG_ENCRYPTION

    auto verify_result = locked_file->DecryptStream(result, obj_number, generation_number);
    if (data != verify_result) {
        assert(!"Stream encryption problem");
    }

#endif /* DEBUG_ENCRYPTION */

    return result;
}

BufferPtr StreamObject::EncryptData(BufferPtr data, types::big_uint obj_number, types::ushort generation_number, NameObjectPtr handler) const {

    auto locked_file = m_file.GetReference();

    // During the initialization it is unknown whether a file is encrypted
    // This is important for object streams that are being parsed before encryption dictionary
    bool is_file_encrypted = locked_file->IsInitialized() && locked_file->IsEncrypted();

    if (IsEncryptionExempted() || !is_file_encrypted) {
        return data;
    }

    auto result = locked_file->EncryptData(data, obj_number, generation_number, handler);

#ifdef DEBUG_ENCRYPTION

    auto verify_result = locked_file->DecryptData(result, obj_number, generation_number, handler);
    if (data != verify_result) {
        assert(!"Stream encryption problem");
    }

#endif /* DEBUG_ENCRYPTION */

    return result;
}

BufferPtr StreamObject::DecryptStream(BufferPtr data, types::big_uint obj_number, types::ushort generation_number) const {

    auto locked_file = m_file.GetReference();

    // During the initialization it is unknown whether a file is encrypted
    // This is important for object streams that are being parsed before encryption dictionary
    bool is_file_encrypted = locked_file->IsInitialized() && locked_file->IsEncrypted();

    if (IsEncryptionExempted() || !is_file_encrypted) {
        return data;
    }

    return locked_file->DecryptStream(data, obj_number, generation_number);
}

BufferPtr StreamObject::DecryptData(BufferPtr data, types::big_uint obj_number, types::ushort generation_number, NameObjectPtr handler) const {

    auto locked_file = m_file.GetReference();

    // During the initialization it is unknown whether a file is encrypted
    // This is important for object streams that are being parsed before encryption dictionary
    bool is_file_encrypted = locked_file->IsInitialized() && locked_file->IsEncrypted();

    if (IsEncryptionExempted() || !is_file_encrypted) {
        return data;
    }

    return locked_file->DecryptData(data, obj_number, generation_number, handler);
}

std::string StreamObject::ToString(void) const {
    // GetBodyEncoded() may load body from file (acquires M0 internally).
    // Compute it before _access_lock to avoid M_stream -> M0 inversion.
    auto encoded = GetBodyEncoded();

    ACCESS_LOCK_GUARD(_access_lock);
    auto stream = StreamUtils::InputOutputStreamFromMemory();
    stream->Write(_header->ToString());
    stream->Write("stream: ");
    stream->WriteLine(std::to_string(encoded->size()));
    return stream->ToString();
}

void StreamObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
    // GetBodyEncoded() may load body from file (acquires M0 internally).
    // Compute it before _access_lock to avoid M_stream -> M0 inversion.
    auto encoded = GetBodyEncoded();

    ACCESS_LOCK_GUARD(_access_lock);
    _header->ToPdfStream(output);
    output << WhiteSpace::LINE_FEED;
    output << "stream";
    output << WhiteSpace::LINE_FEED;
    output << *encoded;
    output << "endstream";
}

size_t StreamObject::Hash() const {
    ACCESS_LOCK_GUARD(_access_lock);
    return _header->Hash();
}

bool StreamObject::Equals(ObjectPtr other) const {
    if (!ObjectUtils::IsType<StreamObjectPtr>(other)) {
        return false;
    }

    auto other_obj = ObjectUtils::ConvertTo<StreamObjectPtr>(other);

    // GetBodyEncoded() may load body from file (acquires M0 internally).
    // Compute both bodies before _access_lock to avoid M_stream -> M0 inversion.
    auto first_body = GetBodyEncoded();
    auto second_body = other_obj->GetBodyEncoded();

    ACCESS_LOCK_GUARD(_access_lock);

    auto first_header = GetHeader();
    auto second_header = other_obj->GetHeader();
    if (!first_header->Equals(second_header)) {
        return false;
    }

    if (first_body != second_body) {
        return false;
    }

    return true;
}

} // syntax
} // vanillapdf
