#include "precompiled.h"
#include "stream_object.h"

#include "syntax/files/file.h"
#include "syntax/filters/filter.h"

#include "syntax/exceptions/syntax_exceptions.h"

namespace gotchangpdf {
namespace syntax {

StreamObject::StreamObject() {
	_header->SetOwner(Object::GetWeakReference());
	_header->Subscribe(this);
	_body->Subscribe(this);
	_body_decoded->Subscribe(this);
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

void StreamObject::ObserveeChanged(IModifyObservable*) {
	OnChanged();
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
}

types::stream_offset StreamObject::GetDataOffset() const {
	return _raw_data_offset;
}

void StreamObject::SetDataOffset(types::stream_offset offset) {
	_raw_data_offset = offset;
	OnChanged();
}

Object::Type StreamObject::GetType(void) const noexcept {
	return Object::Type::Stream;
}

StreamObject* StreamObject::Clone(void) const {
	StreamObjectPtr result(pdf_new StreamObject(), false);

	result->SetFile(m_file);

	result->_body = GetBodyRaw()->Clone();
	result->_body_decoded = _body_decoded->Clone();
	result->_header = _header->Clone();

	result->_body->Subscribe(result.get());
	result->_body_decoded->Subscribe(result.get());
	result->_header->Subscribe(result.get());

	result->_body->SetInitialized();
	result->_body_decoded->SetInitialized();
	result->_header->SetInitialized();
	result->SetInitialized();

	return result.detach();
}

void StreamObject::SetFile(WeakReference<File> file) noexcept {
	Object::SetFile(file);
	_header->SetFile(file);
}

void StreamObject::SetInitialized(bool initialized) noexcept {
	IModifyObservable::SetInitialized(initialized);
	_header->SetInitialized(initialized);
}

BufferPtr StreamObject::GetBodyRaw() const {
	if (!_body->empty()) {
		return _body;
	}

	if (!m_file.IsActive()) {
		throw FileDisposedException();
	}

	auto locked_file = m_file.GetReference();
	auto input = locked_file->GetInputStream();
	auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
	auto pos = input->GetPosition();
	input->SetPosition(_raw_data_offset);

	// We want to capture input by value, because it might be out of scope
	// In order to call non-const method we have to tag the lambda mutable
	auto cleanup_lambda = [input, pos]() mutable {
		input->SetPosition(pos);
	};

	SCOPE_GUARD(cleanup_lambda);

	auto body = input->Read(size->SafeConvert<size_t>());

	if (IsEncryptionExempted() || !locked_file->IsEncrypted()) {
		_body->assign(body.begin(), body.end());
		_body->SetInitialized();
		return _body;
	}

	if (!_header->Contains(constant::Name::Filter)) {
		// Stream does not contain crypt filter
		body = locked_file->DecryptStream(body, GetObjectNumber(), GetGenerationNumber());
		_body->assign(body.begin(), body.end());
		_body->SetInitialized();
		return _body;
	}

	auto filter_obj = _header->Find(constant::Name::Filter);
	bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
	bool is_filter_array = ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

	if (is_filter_name) {
		auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
		if (filter_name == constant::Name::Crypt) {
			auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
			auto handler_name = params->FindAs<NameObjectPtr>(constant::Name::Name);
			body = locked_file->DecryptData(body, GetObjectNumber(), GetGenerationNumber(), handler_name);
			_body->assign(body.begin(), body.end());
			_body->SetInitialized();
			return _body;
		}
	}

	if (is_filter_array) {
		auto filter_array = _header->FindAs<ArrayObjectPtr<NameObjectPtr>>(constant::Name::Filter);
		for (unsigned int i = 0; i < filter_array->Size(); ++i) {
			auto current_filter = (*filter_array)[i];
			if (current_filter == constant::Name::Crypt) {
				assert(i == 0 && "Crypt filter is not first");
				auto params = _header->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::DecodeParms);
				auto handler_name = params->At(i)->FindAs<NameObjectPtr>(constant::Name::Name);
				body = locked_file->DecryptData(body, GetObjectNumber(), GetGenerationNumber(), handler_name);
				_body->assign(body.begin(), body.end());
				_body->SetInitialized();
				return _body;
			}
		}
	}

	// Stream does not contain crypt filter
	body = locked_file->DecryptStream(body, GetObjectNumber(), GetGenerationNumber());
	_body->assign(body.begin(), body.end());
	_body->SetInitialized();
	return _body;
}

BufferPtr StreamObject::GetBody() const {
	if (!_body_decoded->empty())
		return _body_decoded;

	if (!_header->Contains(constant::Name::Filter)) {
		return GetBodyRaw();
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
		return GetBodyRaw();
	}

	if (is_filter_name) {
		auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
		if (filter_name == constant::Name::Crypt) {
			auto body = GetBodyRaw();
			_body_decoded->assign(body.begin(), body.end());
			_body_decoded->SetInitialized();
			return _body_decoded;
		}

		auto filter = FilterBase::GetFilterByName(filter_name);
		if (_header->Contains(constant::Name::DecodeParms)) {
			auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
			//_body_decoded = filter->Decode(*stream, *size, params);
			auto body = filter->Decode(GetBodyRaw(), params);
			_body_decoded->assign(body.begin(), body.end());
			_body_decoded->SetInitialized();
			return _body_decoded;
		}

		//_body_decoded = filter->Decode(*stream, *size);
		auto body = filter->Decode(GetBodyRaw());
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
			assert(filter_array->Size() == params->Size());
		}

		BufferPtr result = GetBodyRaw();
		for (unsigned int i = 0; i < filter_array->Size(); ++i) {
			auto current_filter = (*filter_array)[i];
			if (current_filter == constant::Name::Crypt) {
				continue;
			}

			auto filter = FilterBase::GetFilterByName(current_filter);

			if (has_params) {
				auto current_param = (*params)[i];
				bool is_param_null = ObjectUtils::IsType<NullObjectPtr>(current_param);
				if (is_param_null) {
					result = filter->Decode(result);
					continue;
				}

				auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(current_param);
				result = filter->Decode(result, dict);
				continue;
			}

			result = filter->Decode(result);
		}

		_body_decoded->assign(result.begin(), result.end());
		_body_decoded->SetInitialized();
		return _body_decoded;
	}

	assert(is_filter_name ^ is_filter_array);
	throw GeneralException("Filter is neither name nor array of names");
}

BufferPtr StreamObject::GetBodyEncoded() const {
	// Optimization for unchanged streams
	if (!IsDirty()) {
		return GetBodyRaw();
	}

	if (!_header->Contains(constant::Name::Filter)) {
		return GetBody();
	}

	BufferPtr decoded_body = GetBody();
	auto filter_obj = _header->Find(constant::Name::Filter);
	bool is_filter_null = ObjectUtils::IsType<NullObjectPtr>(filter_obj);
	bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
	bool is_filter_array = ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

	if (is_filter_null) {
		return decoded_body;
	}

	if (is_filter_name) {
		auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
		if (filter_name == constant::Name::Crypt) {
			return decoded_body;
		}

		auto filter = FilterBase::GetFilterByName(filter_name);
		if (_header->Contains(constant::Name::DecodeParms)) {
			auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
			return filter->Encode(decoded_body, params);
		}

		return filter->Encode(decoded_body);
	}

	if (is_filter_array) {
		auto filter_array = _header->FindAs<ArrayObjectPtr<NameObjectPtr>>(constant::Name::Filter);

		MixedArrayObjectPtr params;
		bool has_params = _header->Contains(constant::Name::DecodeParms);
		if (has_params) {
			params = _header->FindAs<MixedArrayObjectPtr>(constant::Name::DecodeParms);
			assert(filter_array->Size() == params->Size());
		}

		for (unsigned int i = 0; i < filter_array->Size(); ++i) {
			auto current_filter = (*filter_array)[i];
			if (current_filter == constant::Name::Crypt) {
				continue;
			}

			auto filter = FilterBase::GetFilterByName(current_filter);

			if (has_params) {
				auto current_param = (*params)[i];
				bool is_param_null = ObjectUtils::IsType<NullObjectPtr>(current_param);
				if (is_param_null) {
					decoded_body = filter->Encode(decoded_body);
					continue;
				}

				auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(current_param);
				decoded_body = filter->Encode(decoded_body, dict);
				continue;
			}

			decoded_body = filter->Encode(decoded_body);
		}

		return decoded_body;
	}

	assert(is_filter_name ^ is_filter_array);
	throw GeneralException("Filter is neither name nor array of names");
}

std::string StreamObject::ToString(void) const {
	std::stringstream ss;
	ss << _header->ToString() << "stream: " << GetBodyEncoded()->size() << std::endl;
	return ss.str();
}

std::string StreamObject::ToPdf(void) const {
	std::stringstream ss;
	ss << _header->ToPdf() << std::endl;
	ss << "stream" << std::endl;
	ss << GetBodyEncoded()->ToString();
	ss << "endstream";
	return ss.str();
}

size_t StreamObject::Hash() const {
	auto decoded_body = GetBody();
	return _header->Hash() ^ decoded_body->Hash();
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

	auto first_body = GetBody();
	auto second_body = other_obj->GetBody();
	if (first_body != second_body) {
		return false;
	}

	return true;
}

} // syntax
} // gotchangpdf
