#include "precompiled.h"
#include "stream_object.h"

#include "file.h"
#include "raw_stream.h"
#include "filter.h"

namespace gotchangpdf
{
	namespace syntax
	{
		StreamObject::StreamObject(DictionaryObjectPtr header, types::stream_offset offset) : _header(header), _raw_data_offset(offset) {}

		BufferPtr StreamObject::GetBody() const
		{
			if (!_body->empty())
				return _body;

			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			auto input = locked_file->GetInputStream();
			auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
			auto stream = Stream(*input);
			auto pos = stream.tellg();
			stream.seekg(_raw_data_offset);
			_body = stream.read(*size);
			stream.seekg(pos);

			return _body;
		}

		BufferPtr StreamObject::GetBodyDecoded() const
		{
			if (!_body_decoded->empty())
				return _body_decoded;

			BufferPtr result = GetBody();
			if (_header->Contains(constant::Name::Filter)) {
				auto filter_obj = _header->Find(constant::Name::Filter);
				bool is_filter_name = ObjectUtils::IsType<NameObjectPtr>(filter_obj);
				bool is_filter_array = ObjectUtils::IsType<MixedArrayObjectPtr>(filter_obj);

				assert(is_filter_name ^ is_filter_array);

				if (is_filter_name) {
					auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
					auto filter = FilterBase::GetFilterByName(filter_name);
					if (_header->Contains(constant::Name::DecodeParms)) {
						auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
						return filter->Decode(result, params);
					}

					return filter->Decode(result);
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
						auto filter = FilterBase::GetFilterByName(current_filter);

						if (has_params) {
							auto current_param = (*params)[i];
							bool is_param_null = ObjectUtils::IsType<NullObjectPtr>(current_param);
							if (is_param_null) {
								result = filter->Decode(result);
							}
							else {
								auto dict = ObjectUtils::ConvertTo<DictionaryObjectPtr>(current_param);
								result = filter->Decode(result, dict);
							}
						}
						else {
							result = filter->Decode(result);
						}
					}
				}
			}

			std::swap(result, _body_decoded);
			return _body_decoded;
		}

		std::string StreamObject::ToString(void) const
		{
			std::stringstream ss;
			ss << _header << "stream: " << _body->size() << std::endl;
			return ss.str();
		}
	}
}
