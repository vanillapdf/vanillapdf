#include "indirect_object_reference.h"
#include "File.h"
#include "gotchangpdf.h"

#include <string>
#include <algorithm>

namespace gotchangpdf
{
	using namespace std;

	IndirectObjectReference::IndirectObjectReference(std::shared_ptr<files::File> file) : Object(Object::Type::IndirectReference), _file(file) {}

	IndirectObjectReference::IndirectObjectReference(std::shared_ptr<files::File> file, int obj_number, int gen_number) :
		Object(Object::Type::IndirectReference), _file(file), _obj_number(obj_number), _gen_number(gen_number) {}

	ObjectReferenceWrapper<IndirectObject> IndirectObjectReference::GetReferencedObject() const
	{
		if (nullptr == _reference)
		{
			// TODO
			_reference = _file->GetIndirectObject(_obj_number, _gen_number);
		}

		return _reference;
	}
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectReferenceHandle handle)
{
	gotchangpdf::IndirectObjectReference* obj = reinterpret_cast<gotchangpdf::IndirectObjectReference*>(handle);
	return reinterpret_cast<IndirectObjectHandle>(obj->GetReferencedObject().AddRefGet());
}
