#include "page_object.h"

#include "name_object.h"
#include "integer_object.h"
#include "exception.h"

#include "c_page_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace exceptions;
		using namespace constant;

		PageObject::PageObject(ObjectReferenceWrapper<DictionaryObject> obj) :
			PageNode(obj),
			_parent(Bind<DictionaryObject, PageTreeNode>(_obj, std::bind(&PageObject::GetParent, this, _obj))),
			_resources(Bind<DictionaryObject, ResourceDictionary>(_obj, std::bind(&PageObject::GetResources, this, _obj))),
			_media_box(Bind<DictionaryObject, Rectangle>(_obj, std::bind(&PageObject::GetMediaBox, this, _obj)))
		{
			if (*_obj->FindAs<NameObject>(Name::Type) != Name::Page)
				throw Exception("TODO");
		}

		ObjectReferenceWrapper<PageTreeNode> PageObject::GetParent(ObjectReferenceWrapper<DictionaryObject> obj)
		{
			auto node = new PageTreeNode(_obj->FindAs<DictionaryObject>(Name::Parent));
			return ObjectReferenceWrapper<PageTreeNode>(node);
		}

		ObjectReferenceWrapper<ResourceDictionary> PageObject::GetResources(ObjectReferenceWrapper<DictionaryObject> obj)
		{
			auto resource = _obj->FindAs<DictionaryObject>(Name::Resources);
			auto dict = new ResourceDictionary(resource);

			return ObjectReferenceWrapper<ResourceDictionary>(dict);
		}

		ObjectReferenceWrapper<Rectangle> PageObject::GetMediaBox(ObjectReferenceWrapper<DictionaryObject> obj)
		{
			auto box = _obj->FindAs<MixedArrayObject>(Name::MediaBox);
			auto specialized = box->ToArrayType<IntegerObject>();
			auto rectangle = new Rectangle(ObjectReferenceWrapper<ArrayObject<IntegerObject>>(specialized));

			return ObjectReferenceWrapper<Rectangle>(rectangle);
		}
	}
}

GOTCHANG_PDF_API void CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	gotchangpdf::documents::PageObject* obj = reinterpret_cast<gotchangpdf::documents::PageObject*>(handle);
	obj->Release();
}
