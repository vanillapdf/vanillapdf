#include "precompiled.h"
#include "stream_object.h"

#include "file.h"
#include "raw_stream.h"
#include "filter.h"
#include "filter_utils.h"
#include "filter_visitors.h"
#include "object_visitors.h"

namespace gotchangpdf
{
	namespace syntax
	{
		StreamObject::StreamObject(DictionaryObjectPtr header, types::stream_offset offset) : _header(header), _raw_data_offset(offset) {}

		BufferPtr StreamObject::GetBody() const
		{
			if (!_body->empty())
				return _body;

			auto input = _file->GetInputStream();
			if (auto locked = input.lock()) {
				auto size = _header->FindAs<IntegerObjectPtr>(constant::Name::Length);
				auto stream = Stream(*locked);
				auto pos = stream.tellg();
				stream.seekg(_raw_data_offset);
				_body = stream.read(*size);
				stream.seekg(pos);
			}
			else {
				throw Exception("Could not obtain fstream lock");
			}

			return _body;
		}

		BufferPtr StreamObject::GetBodyDecoded() const
		{
			if (!_body_decoded->empty())
				return _body_decoded;

			BufferPtr result = GetBody();
			if (_header->Contains(constant::Name::Filter)) {
				auto filter = _header->Find(constant::Name::Filter);

				IsTypeVisitor<NameObjectPtr> name_visitor;
				IsTypeVisitor<MixedArrayObjectPtr> array_visitor;

				bool is_filter_name = filter.apply_visitor(name_visitor);
				bool is_filter_array = filter.apply_visitor(array_visitor);

				assert(is_filter_name ^ is_filter_array);

				if (is_filter_name) {
					FilterBaseVisitor visitor;

					auto filter_name = _header->FindAs<NameObjectPtr>(constant::Name::Filter);
					auto filter_variant = GetFilterByName(filter_name);
					auto filter_base = filter_variant.apply_visitor(visitor);
					if (_header->Contains(constant::Name::DecodeParms)) {
						auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
						return filter_base->Decode(result, params);
					}

					return filter_base->Decode(result);
				}

				if (is_filter_array) {
					auto filter_array = _header->FindAs<ArrayObjectPtr<NameObjectPtr>>(constant::Name::Filter);

					MixedArrayObjectPtr params;
					bool has_params = _header->Contains(constant::Name::DecodeParms);
					if (has_params) {
						params = _header->FindAs<MixedArrayObjectPtr>(constant::Name::DecodeParms);
						assert(filter_array->Size() == params->Size());
					}

					for (int i = 0; i < filter_array->Size(); ++i) {
						FilterBaseVisitor visitor;

						auto current_filter = (*filter_array)[i];
						auto filter_variant = GetFilterByName(current_filter);
						auto filter_base = filter_variant.apply_visitor(visitor);

						if (has_params) {
							auto current_param = (*params)[i];

							IsTypeVisitor<NullObjectPtr> is_null_visitor;
							bool is_param_null = current_param.apply_visitor(is_null_visitor);
							if (is_param_null) {
								result = filter_base->Decode(result);
							}
							else {
								ObjectVisitor<DictionaryObjectPtr> dictionary_convert;
								auto dict = current_param.apply_visitor(dictionary_convert);
								result = filter_base->Decode(result, dict);
							}
						}
						else {
							result = filter_base->Decode(result);
						}
					}
				}
			}

			std::swap(result, _body_decoded);
			return _body_decoded;
		}
	}
}
