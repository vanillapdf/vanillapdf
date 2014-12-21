#include "indirect_object_reference.h"

#include "file.h"
#include "objects.h"

#include "c_indirect_object_reference.h"

namespace gotchangpdf
{
	IndirectObjectReference::IndirectObjectReference(files::File * file, IntegerObjectPtr obj_number, IntegerObjectPtr gen_number) :
		_file(file), _obj_number(obj_number), _gen_number(gen_number)
	{
		assert(nullptr != _file);

		if (_file->IsIndirectObjectIntialized(_obj_number->Value(), _gen_number->Value())) {
			_object = _file->GetIndirectObject(_obj_number->Value(), _gen_number->Value());
			_initialized = true;
		}
		else {
			_initialized = false;
		}
	}

	void IndirectObjectReference::SetObject(files::File * file,
		IntegerObjectPtr obj_number,
		IntegerObjectPtr gen_number)
	{
		assert(nullptr != file);

		_file = file;
		if (_file->IsIndirectObjectIntialized(_obj_number->Value(), _gen_number->Value())) {
			_object = _file->GetIndirectObject(_obj_number->Value(), _gen_number->Value());
			_initialized = true;
		}
		else {
			_initialized = false;
		}
	}

	void IndirectObjectReference::SetObject(files::File * file, IndirectObjectPtr obj)
	{
		assert(nullptr != file);

		_file = file;
		_object = obj;
		_initialized = true;
	}

	IndirectObjectPtr IndirectObjectReference::GetReferencedObject() const
	{
		if (!_initialized) {
			_object = _file->GetIndirectObject(_obj_number->Value(), _gen_number->Value());
			_initialized = true;
		}

		return _object;
	}

	bool IndirectObjectReference::operator<(const IndirectObjectReference& other) const
	{
		if (*_obj_number != *other._obj_number)
			return *_obj_number < *other._obj_number;

		if (*_gen_number != *other._gen_number)
			return *_gen_number < *other._gen_number;

		return false;
	}
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle handle)
{
	gotchangpdf::IndirectObjectReference* obj = reinterpret_cast<gotchangpdf::IndirectObjectReference*>(handle);
	return reinterpret_cast<IndirectObjectHandle>(AddRefGet(obj->GetReferencedObject()));
}
