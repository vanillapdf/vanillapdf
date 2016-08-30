#include "precompiled.h"
#include "page_node_utils.h"

#include "exception.h"
#include "name_object.h"
#include "dictionary_object.h"
#include "page_tree_node.h"
#include "page_object.h"
#include "semantic_exceptions.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;

		PageNodeBasePtr CreatePageNode(syntax::DictionaryObjectPtr obj, WeakReference<Document> doc)
		{
			if (!obj->Contains(Name::Type))
				throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageNodeBase>(obj);

			auto type = obj->FindAs<syntax::NameObjectPtr>(Name::Type);

			if (type == Name::Pages) {
				auto result = PageTreeNodePtr(obj);
				result->SetDocument(doc);
				return result;
			}

			if (type == Name::Page) {
				auto result = PageObjectPtr(obj);
				result->SetDocument(doc);
				return result;
			}

			throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageNodeBase>(obj);
		}
	}
}

