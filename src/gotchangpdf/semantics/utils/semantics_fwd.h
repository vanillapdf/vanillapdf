#ifndef _SEMANTICS_FWD_H
#define _SEMANTICS_FWD_H

#include "syntax_fwd.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class Document; using DocumentPtr = Deferred<Document>;
		class Catalog; using CatalogPtr = Deferred<Catalog>;
		class PageNodeBase; using PageNodeBasePtr = Deferred<PageNodeBase>;
		class PageTree; using PageTreePtr = Deferred<PageTree>;
		class PageTreeNode; using PageTreeNodePtr = Deferred<PageTreeNode>;
		class PageObject; using PageObjectPtr = Deferred<PageObject>;

		class ResourceDictionary; using ResourceDictionaryPtr = Deferred<ResourceDictionary>;

		class Rectangle; using RectanglePtr = Deferred<Rectangle>;

		class Date;
		class NameTree;
		class NumberTree;
		class Rectangle;
		class Tree;

		class ContentStream; using ContentStreamPtr = Deferred<ContentStream>;
		class Contents; using ContentsPtr = Deferred<Contents>;

		class DeveloperExtensions; using DeveloperExtensionsPtr = Deferred<DeveloperExtensions>;

		namespace contents
		{
			class ContentObjectBase; using ContentObjectBasePtr = Deferred<ContentObjectBase>;
			class OperationBeginText; using OperationBeginTextPtr = Deferred<OperationBeginText>;
			class OperationEndText; using OperationEndTextPtr = Deferred<OperationEndText>;
			class OperationTextShow; using OperationTextShowPtr = Deferred<OperationTextShow>;
			class TextObject; using TextObjectPtr = Deferred<TextObject>;
		}
	}
}

#endif /* _SEMANTICS_FWD_H */
