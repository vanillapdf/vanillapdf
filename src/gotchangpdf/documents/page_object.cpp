#include "page_object.h"

#include "name_object.h"
#include "integer_object.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace exceptions;
		using namespace constant;

		PageObject::PageObject(DictionaryObjectPtr obj) :
			PageNode(obj),
			_parent(Bind<PageTreeNodePtr>(_obj, std::bind(&PageObject::GetParent, this, _obj))),
			_resources(Bind<ResourceDictionaryPtr>(_obj, std::bind(&PageObject::GetResources, this, _obj))),
			_media_box(Bind<RectanglePtr>(_obj, std::bind(&PageObject::GetMediaBox, this, _obj)))
		{
			if (*_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Page)
				throw Exception("TODO");
		}

		PageTreeNodePtr PageObject::GetParent(DictionaryObjectPtr obj)
		{
			auto node = new PageTreeNode(_obj->FindAs<DictionaryObjectPtr>(Name::Parent));
			return node;
		}

		ResourceDictionaryPtr PageObject::GetResources(DictionaryObjectPtr obj)
		{
			auto resource = _obj->FindAs<DictionaryObjectPtr>(Name::Resources);
			auto dict = new ResourceDictionary(resource);

			return dict;
		}

		RectanglePtr PageObject::GetMediaBox(DictionaryObjectPtr obj)
		{
			auto box = _obj->FindAs<MixedArrayObjectPtr>(Name::MediaBox);
			auto specialized = box->CastToArrayType<IntegerObjectPtr>();
			auto rectangle = new Rectangle(Deferred<ArrayObject<IntegerObjectPtr>>(specialized));

			return rectangle;
		}
	}
}
