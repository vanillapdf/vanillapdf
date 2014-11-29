#include "indirect_object.h"

#include "file.h"
#include "spirit_parser.h"
#include "object_visitors.h"

#include "c_indirect_object.h"

#include <string>
#include <algorithm>

namespace gotchangpdf
{
	using namespace std;

	IndirectObject::IndirectObject(files::File * file)
		: _file(file),
		  RequireVersion(file->GetHeader()->GetVersion()) {}

	IndirectObject::IndirectObject(const IndirectObject& other)
		: _file(other._file),
		  _gen_number(other._gen_number),
		  _obj_number(other._obj_number),
		  _offset(other._offset),
		  _reference(other._reference),
		  RequireVersion(other._file->GetHeader()->GetVersion()) {}

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
				parser.seekg(_offset);
				auto obj = parser.readObjectWithType<IndirectObject>();
				parser.seekg(pos);
				_reference = obj->GetObject();
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

using namespace gotchangpdf;

GOTCHANG_PDF_API long long CALLING_CONVENTION IndirectObject_GetOffset(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);
	return obj->GetOffset();
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION IndirectObject_GetObject(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);

	gotchangpdf::DirectObject item = obj->GetObject();
	gotchangpdf::ObjectBaseVisitor visitor;

	gotchangpdf::Object *ptr = item.apply_visitor(visitor).AddRefGet();
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API void CALLING_CONVENTION IndirectObject_Release(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);
	obj->Release();
}
