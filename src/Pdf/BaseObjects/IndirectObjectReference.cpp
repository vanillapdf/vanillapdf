#include "IndirectObjectReference.h"
#include "CharacterSet.h"
#include "File.h"

#include <string>
#include <algorithm>

namespace Pdf
{
	using namespace std;

	IndirectObjectReference::IndirectObjectReference(std::shared_ptr<Files::File> file) : Object(Object::Type::IndirectReference), _file(file), _reference(nullptr) {}

	IndirectObjectReference::IndirectObjectReference(std::shared_ptr<Files::File> file, int obj_number, int gen_number) : Object(Object::Type::IndirectReference), _file(file)
	{
		_obj_number = obj_number;
		_gen_number = gen_number;

		// TODO
		//_reference = _file.GetIndirectObject(objNumber, genNumber);
	}

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

