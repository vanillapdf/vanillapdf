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
			enum class Type : unsigned char
			{
				UNKNOWN = 0,
				PAGE_TREE_NODE,
				PAGE_OBJECT_NODE
			};

			void Release();

			virtual Type GetType() const = 0;

			virtual ~PageNode() = 0;

		private:
			mutable long _intrusive_ref_count;

			template <typename T>
			friend void ::boost::intrusive_ptr_add_ref(T*);

			template <typename T>
			friend void ::boost::intrusive_ptr_release(T*);
		};

		class PageTreeNode : public PageNode
		{
		public:
			//PageTreeNode();
			explicit PageTreeNode(const IndirectObject& obj);

			inline ObjectReferenceWrapper<IntegerObject> Count(void) const { return _count; }
			inline ObjectReferenceWrapper<ArrayObject> Kids(void) const { return _kids; }

			virtual Type GetType() const override { return PageNode::Type::PAGE_TREE_NODE; }

		private:
			ObjectReferenceWrapper<IntegerObject> _count;
			ObjectReferenceWrapper<ArrayObject> _kids;
		};

		class PageObject : public PageNode
		{
		public:
			//PageObject();
			explicit PageObject(const IndirectObject& obj);

			inline ObjectReferenceWrapper<IndirectObjectReference> Parent(void) const { return _parent; }
			inline ObjectReferenceWrapper<DictionaryObject> Resources(void) const { return _resources; }

			virtual Type GetType() const override { return PageNode::Type::PAGE_OBJECT_NODE; }

		private:
			ObjectReferenceWrapper<IndirectObjectReference> _parent;
			ObjectReferenceWrapper<DictionaryObject> _resources;
			ObjectReferenceWrapper<Rectangle> _media_box;
		};

		class PageTree
		{
		public:
			//PageTree();
			explicit PageTree(const IndirectObject& root);

			inline ObjectReferenceWrapper<PageNode> GetRoot(void) const { return _root; }

			void Release();

		private:
			ObjectReferenceWrapper<PageNode> _root;

			mutable long _intrusive_ref_count;

			template <typename T>
			friend void ::boost::intrusive_ptr_add_ref(T*);

			template <typename T>
			friend void ::boost::intrusive_ptr_release(T*);
		};
	}
}

#endif /* _PAGE_TREE_H */
