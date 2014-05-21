#include "page_object.h"

#include "name_object.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace exceptions;
		using namespace constant;

		PageObject::PageObject(const DictionaryObject& obj) : _dictionary(new DictionaryObject(obj))
		{
			if (*_dictionary->FindAs<NameObject>(Name::Type) != Name::Page)
				throw Exception("TODO");

			//_parent = _dictionary->FindAs<IndirectObjectReference>(Name::Parent);

			//TODO rectangle
			//_media_box = _dictionary->FindAs<ArrayObject>(Name::MediaBox);

			_resources = new ResourceDictionary(*_dictionary->FindAs<DictionaryObject>(Name::Resources));
		}
	}
}

