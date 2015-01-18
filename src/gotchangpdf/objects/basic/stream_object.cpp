#include "stream_object.h"

#include "file.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "integer_object.h"
#include "raw_stream.h"
#include "object_visitors.h"

// TODO
#include "flate_decode_filter.h"

namespace gotchangpdf
{
	StreamObject::StreamObject(DictionaryObjectPtr dictionary) : _dictionary(dictionary) {}

	BufferPtr StreamObject::GetData() const
	{
		if (_data->empty())
		{
			auto stream = _file->GetInputStream();
			if (auto locked = stream.lock())
			{
				auto size_raw = _dictionary->Find(constant::Name::Length);
				KillIndirectionVisitor<IntegerObjectPtr> visitor;
				IntegerObjectPtr size = size_raw.apply_visitor(visitor);

				size_t len = static_cast<size_t>(*size);

				auto stream = raw::Stream(*locked);
				auto pos = stream.tellg();
				stream.seekg(_raw_data_offset);
				_data = stream.read(len);
				stream.seekg(pos);
			}
			else
			{
				throw exceptions::Exception("Could not obtain fstream lock");
			}
		}

		auto filter = _dictionary->Find(constant::Name::Filter);
		if (filter.empty())
			return _data;

		ObjectVisitor<NameObject> visitor;
		auto filter_name = filter.apply_visitor(visitor);

		// TODO
		filters::FlateDecodeFilter a;
		return a.Decode(_data);
		//return ((Filters::FlateDecodeFilter*)(&*filter_name))->Apply(*_data);
	}
}
