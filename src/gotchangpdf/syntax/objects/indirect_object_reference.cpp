#include "precompiled.h"
#include "indirect_object_reference.h"

#include "file.h"
#include "objects.h"

namespace gotchangpdf
{
	namespace syntax
	{
		IndirectObjectReference::IndirectObjectReference(ObjectPtr obj)
		{
			assert(obj->IsIndirect());
			_ref_obj = obj->GetObjectNumber();
			_ref_gen = obj->GetGenerationNumber();
		}

		IndirectObjectReference::IndirectObjectReference(types::integer obj, types::ushort gen) : _ref_obj(obj), _ref_gen(gen) {}

		ObjectPtr IndirectObjectReference::GetReferencedObject() const
		{
			auto locked_file = _file.lock();
			if (!locked_file)
				throw FileDisposedException();

			return locked_file->GetIndirectObject(_ref_obj, _ref_gen);
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
}
