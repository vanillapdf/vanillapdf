#include "precompiled.h"
#include "page_node_utils.h"

#include "syntax/objects/name_object.h"
#include "syntax/objects/dictionary_object.h"

#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/page_object.h"
#include "semantics/utils/semantic_exceptions.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;

		PageNodeBasePtr CreatePageNode(syntax::DictionaryObjectPtr obj)
		{
			if (!obj->Contains(Name::Type))
				throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageNodeBase>(obj);

			auto type = obj->FindAs<syntax::NameObjectPtr>(Name::Type);

			if (type == Name::Pages) {
				auto result = PageTreeNodePtr(obj);
				return result;
			}

			if (type == Name::Page) {
				auto result = PageObjectPtr(obj);
				return result;
			}

			throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageNodeBase>(obj);
		}
	}
}

