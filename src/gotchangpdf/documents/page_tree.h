#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "fwd.h"
#include "integer_object.h"
#include "array_object.h"
#include "indirect_object_reference.h"
#include "rectangle.h"
#include "object_reference_wrapper.h"

#include <vector>
#include <memory>

namespace gotchangpdf
{
	namespace documents
	{
		class PageNode
		{
		public:
			virtual ~PageNode() = 0;
		};

		class PageTreeNode : public PageNode
		{
		public:
			PageTreeNode();
			explicit PageTreeNode(const IndirectObject& obj);

		private:
			ObjectReferenceWrapper<IntegerObject> _count;
			ObjectReferenceWrapper<ArrayObject> _kids;
		};

		class PageObject : public PageNode
		{
		public:
			PageObject();
			explicit PageObject(const IndirectObject& obj);

		private:
			ObjectReferenceWrapper<IndirectObjectReference> _parent;
			ObjectReferenceWrapper<DictionaryObject> _resources;
			ObjectReferenceWrapper<Rectangle> _media_box;
		};

		class PageTree
		{
		public:
			PageTree();
			explicit PageTree(const IndirectObject& root);

		private:
			std::vector<std::shared_ptr<PageNode>> _nodes;
		};
	}
}

#endif /* _PAGE_TREE_H */
