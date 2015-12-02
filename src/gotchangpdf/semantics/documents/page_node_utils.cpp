#include "precompiled.h"
#include "page_node_utils.h"

#include "exception.h"
#include "name_object.h"
#include "dictionary_object.h"
#include "page_tree_node.h"
#include "page_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;

		PageNodePtr CreatePageNode(syntax::DictionaryObjectPtr obj)
		{
			auto type = obj->FindAs<syntax::NameObjectPtr>(Name::Type);

			if (*type == Name::Pages)
				return PageTreeNodePtr(pdf_new PageTreeNode(obj));
			else if (*type == Name::Page)
				return PageObjectPtr(pdf_new PageObject(obj));
			else
				throw syntax::Exception("Cannot initialize PageTree from TODO");
		}
	}
}

