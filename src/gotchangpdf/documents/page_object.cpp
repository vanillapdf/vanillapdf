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

		PageObject::PageObject(SmartPtr<DictionaryObject> obj) :
			PageNode(obj),
			_parent(Bind<DictionaryObject, PageTreeNode>(_obj, std::bind(&PageObject::GetParent, this, _obj))),
			_resources(Bind<DictionaryObject, ResourceDictionary>(_obj, std::bind(&PageObject::GetResources, this, _obj))),
			_media_box(Bind<DictionaryObject, Rectangle>(_obj, std::bind(&PageObject::GetMediaBox, this, _obj)))
		{
			if (*_obj->FindAs<NameObject>(Name::Type) != Name::Page)
				throw Exception("TODO");
		}

		SmartPtr<PageTreeNode> PageObject::GetParent(SmartPtr<DictionaryObject> obj)
		{
			auto node = new PageTreeNode(_obj->FindAs<DictionaryObject>(Name::Parent));
			return SmartPtr<PageTreeNode>(node);
		}

		SmartPtr<ResourceDictionary> PageObject::GetResources(SmartPtr<DictionaryObject> obj)
		{
			auto resource = _obj->FindAs<DictionaryObject>(Name::Resources);
			auto dict = new ResourceDictionary(resource);

			return SmartPtr<ResourceDictionary>(dict);
		}

		SmartPtr<Rectangle> PageObject::GetMediaBox(SmartPtr<DictionaryObject> obj)
		{
			auto box = _obj->FindAs<MixedArrayObject>(Name::MediaBox);
			auto specialized = box->CastToArrayType<IntegerObject>();
			auto rectangle = new Rectangle(SmartPtr<ArrayObject<IntegerObject>>(specialized));

			return SmartPtr<Rectangle>(rectangle);
		}
	}
}

GOTCHANG_PDF_API void CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	gotchangpdf::documents::PageObject* obj = reinterpret_cast<gotchangpdf::documents::PageObject*>(handle);
	obj->Release();
}
