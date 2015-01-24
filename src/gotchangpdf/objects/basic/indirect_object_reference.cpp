#include "precompiled.h"
#include "indirect_object_reference.h"

#include "file.h"
#include "objects.h"

namespace gotchangpdf
{
	IndirectObjectReference::IndirectObjectReference(DirectObject obj) : _object(obj) {}

	DirectObject IndirectObjectReference::GetReferencedObject() const
	{
		if (!_initialized) {
			_object = _file->GetIndirectObject(_ref_obj->Value(), _ref_gen->Value());
			_initialized = true;
		}

		return _object;
	}

	bool IndirectObjectReference::Equals(const IndirectObjectReference& other) const
	{
		return _ref_obj->Equals(*other._ref_obj) && _ref_gen->Equals(*other._ref_gen);
		return false;
	}

	bool IndirectObjectReference::operator<(const IndirectObjectReference& other) const
	{
		if (*_ref_obj != *other._ref_obj)
			return *_ref_obj < *other._ref_obj;

		if (*_ref_gen != *other._ref_gen)
			return *_ref_gen < *other._ref_gen;

		return false;
	}
}
