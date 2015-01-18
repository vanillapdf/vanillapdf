#include "indirect_object_reference.h"

#include "file.h"
#include "objects.h"

namespace gotchangpdf
{
	IndirectObjectReference::IndirectObjectReference(files::File * file, IntegerObjectPtr obj_number, IntegerObjectPtr gen_number) :
		_obj_number(obj_number), _gen_number(gen_number)
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

	void IndirectObjectReference::SetObject(files::File * file, DirectObject obj)
	{
		assert(nullptr != file);

		_file = file;
		_object = obj;
		_initialized = true;
	}

	DirectObject IndirectObjectReference::GetReferencedObject() const
	{
		if (!_initialized) {
			_object = _file->GetIndirectObject(_obj_number->Value(), _gen_number->Value());
			_initialized = true;
		}

		return _object;
	}

	bool IndirectObjectReference::Equals(const IndirectObjectReference& other) const
	{
		return _obj_number->Equals(*other._obj_number) && _gen_number->Equals(*other._gen_number);
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
