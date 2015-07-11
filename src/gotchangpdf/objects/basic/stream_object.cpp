#include "precompiled.h"
#include "stream_object.h"

#include "file.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "integer_object.h"
#include "raw_stream.h"

// TODO
#include "flate_decode_filter.h"

namespace gotchangpdf
{
	BufferPtr StreamObject::GetBody() const
	{
		if (!_body->empty())
			return _body;

		auto input = _file->GetInputStream();
		if (auto locked = input.lock())
		{
			auto size_raw = _header->Find(constant::Name::Length);
			KillIndirectionVisitor<IntegerObjectPtr> visitor;
			IntegerObjectPtr size = size_raw.apply_visitor(visitor);

			auto stream = raw::Stream(*locked);
			auto pos = stream.tellg();
			stream.seekg(_raw_data_offset);
			_body = stream.read(*size);
			stream.seekg(pos);
		}
		else
		{
			throw exceptions::Exception("Could not obtain fstream lock");
		}

		return _body;
	}

	BufferPtr StreamObject::GetBodyDecoded() const
	{
		BufferPtr result = GetBody();

		if (_header->Contains(constant::Name::Filter)) {
			auto filter = _header->Find(constant::Name::Filter);

			IsTypeVisitor<NameObjectPtr> nameVisitor;
			IsTypeVisitor<MixedArrayObjectPtr> arrayVisitor;

			bool isFilterName = filter.apply_visitor(nameVisitor);
			bool isFilterArray = filter.apply_visitor(arrayVisitor);

			assert(isFilterName ^ isFilterArray);

			if (isFilterName) {
				ObjectVisitor<NameObjectPtr> convert_visitor;
				auto filter_name = filter.apply_visitor(convert_visitor);
				auto filterInstance = filters::Filter::GetByName(filter_name);
				if (_header->Contains(constant::Name::DecodeParms)) {
					auto params = _header->FindAs<DictionaryObjectPtr>(constant::Name::DecodeParms);
					return filterInstance->Decode(result, params);
				}

				return filterInstance->Decode(result);
			}

			if (isFilterArray) {
				assert(false); //TODO
			}
		}

		return result;
	}
}
