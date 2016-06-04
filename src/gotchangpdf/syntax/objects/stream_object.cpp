#include "precompiled.h"
#include "stream_object.h"

#include "file.h"
#include "raw_stream.h"
#include "filter.h"

namespace gotchangpdf
{
	namespace syntax
	{
		StreamObject::StreamObject(DictionaryObjectPtr header, types::stream_offset offset)
			: _header(header), _raw_data_offset(offset) {}

		BufferPtr StreamObject::GetBodyRaw() const
		{
			if (!_body->empty())
				return _body;

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			auto input = locked_file->GetInputStream();
			auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
			auto pos = input->tellg();
			input->seekg(_raw_data_offset);
			SCOPE_GUARD_CAPTURE_VALUES(input->seekg(pos));
			auto stream = Stream(*input);
			auto body = stream.read(size->SafeConvert<size_t>());

			if (!locked_file->IsEncrypted()) {
				_body = body;
				return _body;
			}

			if (!_header->Contains(constant::Name::Filter)) {
				// Stream does not contain crypt filter
				_body = locked_file->DecryptStream(body, _obj_number, _gen_number);
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
					_body = locked_file->DecryptData(body, _obj_number, _gen_number, handler_name);
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
						_body = locked_file->DecryptData(body, _obj_number, _gen_number, handler_name);
						return _body;
					}
				}
			}

			// Stream does not contain crypt filter
			_body = locked_file->DecryptStream(body, _obj_number, _gen_number);
			return _body;
		}

		BufferPtr StreamObject::GetBody() const
		{
			if (!_body_decoded->empty())
				return _body_decoded;

			if (!_header->Contains(constant::Name::Filter)) {
				return GetBodyRaw();
			}

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			//auto stream = locked_file->GetInputStream();
			//auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
			//auto pos = stream->tellg();
			//stream->seekg(_raw_data_offset);
			//SCOPE_GUARD_CAPTURE_VALUES(stream->seekg(pos));

			auto filter_obj = _header->Find(constant::Name::Filter);
			bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
			bool is_filter_array = ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

			if (is_filter_name) {
				auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
				if (filter_name == constant::Name::Crypt) {
					_body_decoded = GetBodyRaw();
					return _body_decoded;
				}

				auto filter = FilterBase::GetFilterByName(filter_name);
				if (_header->Contains(constant::Name::DecodeParms)) {
					auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
					//_body_decoded = filter->Decode(*stream, *size, params);
					_body_decoded = filter->Decode(GetBodyRaw(), params);
					return _body_decoded;
				}

				//_body_decoded = filter->Decode(*stream, *size);
				_body_decoded = filter->Decode(GetBodyRaw());
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

				_body_decoded = result;
				return _body_decoded;
			}

			assert(is_filter_name ^ is_filter_array);
			throw GeneralException("Filter is neither name nor array of names");
		}

		std::string StreamObject::ToString(void) const
		{
			std::stringstream ss;
			ss << _header->ToString() << "stream: " << GetBodyRaw()->size() << std::endl;
			return ss.str();
		}

		std::string StreamObject::ToPdf(void) const
		{
			std::stringstream ss;
			ss << _header->ToPdf() << std::endl;
			ss << "stream" << std::endl;
			ss << GetBodyRaw()->ToString();
			ss << "endstream";
			return ss.str();
		}
	}
}
