#include "page_object.h"

#include "page_tree_node.h"
#include "resource_dictionary.h"
#include "rectangle.h"
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
			PageNode(obj)
		{
			if (*_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Page)
				throw Exception("TODO");
		}

		PageTreeNodePtr PageObject::Parent() const
		{
			auto node = new PageTreeNode(_obj->FindAs<DictionaryObjectPtr>(Name::Parent));
			return node;
		}

		ResourceDictionaryPtr PageObject::Resources() const
		{
			auto resource = _obj->FindAs<DictionaryObjectPtr>(Name::Resources);
			auto dict = new ResourceDictionary(resource);

			return dict;
		}

		RectanglePtr PageObject::MediaBox() const
		{
			auto box = _obj->FindAs<MixedArrayObjectPtr>(Name::MediaBox);
			auto specialized = box->CastToArrayType<IntegerObjectPtr>();
			auto rectangle = new Rectangle(ArrayObjectPtr<IntegerObjectPtr>(specialized));

			return rectangle;
		}
	}
}
