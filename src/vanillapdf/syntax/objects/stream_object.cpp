#include "precompiled.h"
#include "stream_object.h"

#include "syntax/files/file.h"
#include "syntax/filters/filter.h"
#include "syntax/utils/name_constants.h"

#include "syntax/exceptions/syntax_exceptions.h"

#include <sstream>

namespace vanillapdf {
namespace syntax {

StreamObject::StreamObject() : StreamObject(false) {
}

StreamObject::StreamObject(bool initialized) {
	_header->SetOwner(Object::GetWeakReference());
	_header->Subscribe(this);
	_body->Subscribe(this);
	_body_decoded->Subscribe(this);

	SetInitialized(initialized);
}

StreamObject::StreamObject(DictionaryObjectPtr header, types::stream_offset offset)
	: _header(header), _raw_data_offset(offset) {
	_header->SetOwner(Object::GetWeakReference());
	_header->Subscribe(this);
	_body->Subscribe(this);
	_body_decoded->Subscribe(this);
}

StreamObject::~StreamObject() {
	_header->Unsubscribe(this);
	_body->Unsubscribe(this);
	_body_decoded->Unsubscribe(this);
}

void StreamObject::ObserveeChanged(const IModifyObservable*) {
	OnChanged();
}

void StreamObject::OnChanged() {

	Object::OnChanged();

	if (IsDirty()) {
		auto weak_file = GetFile();
		auto file = weak_file.GetReference();
		auto log_scope = file->GetFilenameString();

		auto obj_number = GetRootObjectNumber();
		auto gen_number = GetRootGenerationNumber();

		LOG_DEBUG(log_scope) << "Stream object " << std::dec << obj_number << " " << gen_number << " change triggered, object is dirty";
	}
}

DictionaryObjectPtr StreamObject::GetHeader() const {
	return _header;
}

void StreamObject::SetHeader(DictionaryObjectPtr header) {
	_header->Unsubscribe(this);
	header->Subscribe(this);
	_header = header;
	OnChanged();
}

void StreamObject::SetBody(BufferPtr value) {
	_body_decoded->assign(value.begin(), value.end());
	_body_decoded->SetInitialized();

	// Trigger change event
	OnChanged();
}

types::stream_offset StreamObject::GetDataOffset() const {
	return _raw_data_offset;
}

void StreamObject::SetDataOffset(types::stream_offset offset) {
	_raw_data_offset = offset;
	OnChanged();
}

Object::Type StreamObject::GetObjectType(void) const noexcept {
	return Object::Type::Stream;
}

StreamObject* StreamObject::Clone(void) const {
	StreamObjectPtr result(pdf_new StreamObject(), false);

	result->_header = _header->Clone();
	result->_header->Subscribe(result.get());
	result->_header->SetInitialized();

	result->_body = GetBodyRaw()->Clone();
	result->_body->Subscribe(result.get());
	result->_body->SetInitialized();

	if (_body_decoded->IsInitialized()) {
		result->_body_decoded = GetBody()->Clone();
		result->_body_decoded->Subscribe(result.get());
		result->_body_decoded->SetInitialized();
	}

	result->SetInitialized();

	CloneBaseProperties(result);
	return result.detach();
}

void StreamObject::SetFile(WeakReference<File> file) {
	Object::SetFile(file);
	_header->SetFile(file);
}

void StreamObject::SetInitialized(bool initialized) {
	IModifyObservable::SetInitialized(initialized);
	_header->SetInitialized(initialized);

	// In case the object is already initialized without data offset
	// consider it data to be initialized as well
	if (initialized && _raw_data_offset == constant::BAD_OFFSET) {
		_body->SetInitialized();
	}
}

BufferPtr StreamObject::GetBodyRaw() const {

	if (_body->IsInitialized()) {
		return _body;
	}

	if (!m_file.IsActive()) {
		throw FileDisposedException();
	}

	if (_raw_data_offset == constant::BAD_OFFSET) {
		throw GeneralException("Stream object data offset is not initialized");
	}

	auto locked_file = m_file.GetReference();
	auto input = locked_file->GetInputStream();

	input->ExclusiveInputLock();
	input->SetInputPosition(_raw_data_offset);

	auto pos = input->GetInputPosition();

	auto cleanup_lambda = [&input, pos]() {
		input->SetInputPosition(pos);
		input->ExclusiveInputUnlock();
	};

	SCOPE_GUARD(cleanup_lambda);

	auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
	auto body = input->Read(size->SafeConvert<types::size_type>());

	_body->assign(body.begin(), body.end());
	_body->SetInitialized();
	return _body;
}

BufferPtr StreamObject::GetBody() const {

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
	throw GeneralException("Filter is neither name nor array of names");
}

BufferPtr StreamObject::GetBodyEncoded() const {

	auto locked_file = m_file.GetReference();

	// Optimization for unchanged streams
	// In case the document is encrypted, the stream contents needs to be recalculated
	if (!IsDirty() && !locked_file->IsEncrypted()) {
		return GetBodyRaw();
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
	throw GeneralException("Filter is neither name nor array of names");
}

BufferPtr StreamObject::GetBodyDecrypted() const {

	auto locked_file = m_file.GetReference();
	auto body_raw = GetBodyRaw();

	// During the initialization it is unknown whether a file is encrypted
	// This is important for object streams that are being parsed before encryption dictionary
	bool is_file_encrypted = locked_file->IsInitialized() && locked_file->IsEncrypted();

	if (IsEncryptionExempted() || !is_file_encrypted) {
		return body_raw;
	}

	// Stream does not contain crypt filter
	if (!_header->Contains(constant::Name::Filter)) {
		return locked_file->DecryptStream(body_raw, GetRootObjectNumber(), GetRootGenerationNumber());
	}

	auto filter_obj = _header->Find(constant::Name::Filter);
	bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
	bool is_filter_array = ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

	if (is_filter_name) {
		auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
		if (filter_name == constant::Name::Crypt) {
			auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
			auto handler_name = params->FindAs<NameObjectPtr>(constant::Name::Name);
			return locked_file->DecryptData(body_raw, GetRootObjectNumber(), GetRootGenerationNumber(), handler_name);
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
				return locked_file->DecryptData(body_raw, GetRootObjectNumber(), GetRootGenerationNumber(), handler_name);
			}
		}
	}

	// Stream does not contain crypt filter
	return locked_file->DecryptStream(body_raw, GetRootObjectNumber(), GetRootGenerationNumber());
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
	std::stringstream ss;
	ss << _header->ToString() << "stream: " << GetBodyEncoded()->size() << std::endl;
	return ss.str();
}

void StreamObject::ToPdfStreamInternal(IOutputStreamPtr output) const {
	std::stringstream ss;
	ss << _header->ToPdf() << std::endl;
	ss << "stream" << std::endl;
	ss << GetBodyEncoded()->ToString();
	ss << "endstream";

	output << ss.str();
}

size_t StreamObject::Hash() const {
	return _header->Hash();
}

bool StreamObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<StreamObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<StreamObjectPtr>(other);

	auto first_header = GetHeader();
	auto second_header = other_obj->GetHeader();
	if (!first_header->Equals(second_header)) {
		return false;
	}

	auto first_body = GetBodyEncoded();
	auto second_body = other_obj->GetBodyEncoded();
	if (first_body != second_body) {
		return false;
	}

	return true;
}

} // syntax
} // vanillapdf
