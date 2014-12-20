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

	IndirectObjectReference::IndirectObjectReference(files::File * file, IntegerObjectPtr obj_number, IntegerObjectPtr gen_number) :
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

	bool IndirectObjectReference::operator<(const IndirectObjectReference& other) const
	{
		if (*_obj_number < *other._obj_number)
			return true;

		if (*_gen_number < *other._gen_number)
			return true;

		return false;
	}
}

namespace std
{
	size_t hash<gotchangpdf::IndirectObjectReference>::operator()(const gotchangpdf::IndirectObjectReference& ref) const
	{
		std::hash<gotchangpdf::types::integer> hash_fn;

		size_t result = 0;
		result ^= hash_fn(ref.GetObjectNumber()->Value());
		result ^= hash_fn(ref.GetGenerationNumber()->Value());

		return result;
	}
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle handle)
{
	gotchangpdf::IndirectObjectReference* obj = reinterpret_cast<gotchangpdf::IndirectObjectReference*>(handle);
	return reinterpret_cast<IndirectObjectHandle>(AddRefGet(obj->GetReferencedObject()));
}
