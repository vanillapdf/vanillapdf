#include "precompiled.h"
#include "indirect_object_reference.h"

#include "file.h"
#include "objects.h"

namespace gotchangpdf
{
	IndirectObjectReference::IndirectObjectReference(DirectObject obj) : _object(obj)
	{
		ObjectBaseVisitor visitor;
		auto base = obj.apply_visitor(visitor);

		assert(base->IsIndirect());
		_ref_obj = base->GetObjectNumber();
		_ref_gen = base->GetGenerationNumber();
		_initialized = true;
	}

	IndirectObjectReference::IndirectObjectReference(types::integer obj, types::ushort gen) : _ref_obj(obj), _ref_gen(gen) {}

	DirectObject IndirectObjectReference::GetReferencedObject() const
	{
		if (!_initialized) {
			_object = _file->GetIndirectObject(_ref_obj, _ref_gen);
			_initialized = true;
		}

		return _object;
	}

	bool IndirectObjectReference::Equals(const IndirectObjectReference& other) const
	{
		return _ref_obj == other._ref_obj && _ref_gen == other._ref_gen;
	}

	bool IndirectObjectReference::operator<(const IndirectObjectReference& other) const
	{
		if (_ref_obj != other._ref_obj)
			return _ref_obj < other._ref_obj;

		if (_ref_gen != other._ref_gen)
			return _ref_gen < other._ref_gen;

		return false;
	}
}
