#include "page_object.h"

#include "name_object.h"
#include "exception.h"

#include "c_page_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace exceptions;
		using namespace constant;

		PageObject::PageObject(ObjectReferenceWrapper<DictionaryObject> obj) : PageNode(obj)
		{
			if (*_obj->FindAs<NameObject>(Name::Type) != Name::Page)
				throw Exception("TODO");

			//_parent = _dictionary->FindAs<IndirectObjectReference>(Name::Parent);

			//TODO rectangle
			//_media_box = _dictionary->FindAs<ArrayObject>(Name::MediaBox);

			_resources = new ResourceDictionary(_obj->FindAs<DictionaryObject>(Name::Resources));
		}

		ObjectReferenceWrapper<ResourceDictionary> PageObject::Resources(void) const
		{
			if (_resources->GetContainer() != _obj)
				_resources = new ResourceDictionary(_obj->FindAs<DictionaryObject>(Name::Resources));

			return _resources;
		}
	}
}

GOTCHANG_PDF_API void CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	gotchangpdf::documents::PageObject* obj = reinterpret_cast<gotchangpdf::documents::PageObject*>(handle);
	obj->Release();
}
