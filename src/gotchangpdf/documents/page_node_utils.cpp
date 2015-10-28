#include "precompiled.h"
#include "page_node_utils.h"

#include "exception.h"
#include "name_object.h"
#include "dictionary_object.h"
#include "page_tree_node.h"
#include "page_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;

		PageNodePtr CreatePageNode(DictionaryObjectPtr obj)
		{
			auto type = obj->FindAs<NameObjectPtr>(Name::Type);

			if (*type == Name::Pages)
				return PageTreeNodePtr(new PageTreeNode(obj));
			else if (*type == Name::Page)
				return PageObjectPtr(new PageObject(obj));
			else
				throw exceptions::Exception("Cannot initialize PageTree from TODO");
		}
	}
}

