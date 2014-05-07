#include "indirect_object_reference.h"
#include "File.h"

#include <string>
#include <algorithm>

namespace gotchangpdf
{
	using namespace std;

	IndirectObjectReference::IndirectObjectReference(std::shared_ptr<files::File> file) : Object(Object::Type::IndirectReference), _file(file), _reference(nullptr) {}

	IndirectObjectReference::IndirectObjectReference(std::shared_ptr<files::File> file, int obj_number, int gen_number) :
		Object(Object::Type::IndirectReference), _file(file), _obj_number(obj_number), _gen_number(gen_number) {}

	boost::intrusive_ptr<IndirectObject> IndirectObjectReference::GetReference() const
	{
		if (nullptr == _reference)
		{
			// TODO
			_reference = _file->GetIndirectObject(_obj_number, _gen_number);
		}

		return _reference;
	}
}

