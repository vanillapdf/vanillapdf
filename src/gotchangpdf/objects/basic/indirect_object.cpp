#include "indirect_object.h"

#include "file.h"
#include "header.h"
#include "spirit_parser.h"
#include "object_visitors.h"

#include <string>
#include <algorithm>

namespace gotchangpdf
{
	using namespace std;

	IndirectObject::IndirectObject(files::File * file)
		: _file(file) {}

	IndirectObject::IndirectObject(const IndirectObject& other)
		: _file(other._file),
		  _gen_number(other._gen_number),
		  _obj_number(other._obj_number),
		  _offset(other._offset),
		  _reference(other._reference) {}

	void IndirectObject::SetObject(DirectObject ref) { _reference = ref; }

	DirectObject IndirectObject::GetObject() const
	{
		IsNullVisitor visitor;
		auto is_null = _reference.apply_visitor(visitor);
		if (is_null)
		{
			auto stream = _file->GetInputStream();
			if (auto locked = stream.lock())
			{
				auto parser = lexical::SpiritParser(_file, *locked);
				auto pos = parser.tellg();
				//parser.seekg(_offset);

				auto obj = parser.ReadIndirectObject(_offset);
				_reference = obj->GetObject();
				//parser.seekg(pos);
			}
			else
			{
				throw exceptions::Exception("Could not obtain fstream lock");
			}
		}

		return _reference;
	}

	/*
	IObject* IndirectObject::GetIObject() const
	{
		auto item = GetObject();

		gotchangpdf::Object *ptr = item.AddRefGet();
		//boost::intrusive_ptr_add_ref(ptr);

		return reinterpret_cast<IObject*>(ptr);
	}
	*/
}
