#include "indirect_object_reference.h"

#include "file.h"
#include "objects.h"

#include "c_indirect_object_reference.h"

#include <string>
#include <algorithm>

namespace gotchangpdf
{
	using namespace std;

	IndirectObjectReference::IndirectObjectReference(files::File * file) : _file(file) {}

	IndirectObjectReference::IndirectObjectReference(files::File * file, types::integer obj_number, types::ushort gen_number) :
		_file(file), _obj_number(obj_number), _gen_number(gen_number) {}

	Deferred<IndirectObject> IndirectObjectReference::GetReferencedObject() const
	{
		if (!_reference.HasContents())
		{
			// TODO
			_reference = _file->GetIndirectObject(_obj_number->Value(), _gen_number->Value());
		}

		return _reference;
	}
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle handle)
{
	gotchangpdf::IndirectObjectReference* obj = reinterpret_cast<gotchangpdf::IndirectObjectReference*>(handle);
	return reinterpret_cast<IndirectObjectHandle>(AddRefGet(obj->GetReferencedObject()));
}
