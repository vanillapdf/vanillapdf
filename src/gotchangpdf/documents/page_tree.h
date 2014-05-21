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
		class PageNode : public boost::intrusive_ref_counter<PageNode>
		{
		public:
			enum class Type : unsigned char
			{
				PAGE_TREE_NODE,
				PAGE_OBJECT_NODE
			};

			static ObjectReferenceWrapper<PageNode> Create(const IndirectObject& obj);

			void Release();

			virtual Type GetType() const = 0;

			virtual ~PageNode() = 0;
		};

		class PageTreeNode : public PageNode
		{
		public:
			//PageTreeNode();
			explicit PageTreeNode(const IndirectObject& obj);

			inline IntegerObject::ValueType KidCount(void) const { return _count->Value(); }
			inline ObjectReferenceWrapper<PageNode> Kid(unsigned int number) const { return PageNode::Create(*_kids->At(number)->GetReferencedObject()); }
			inline ObjectReferenceWrapper<PageNode> operator[](unsigned int number) const { return PageNode::Create(*(*_kids)[number]->GetReferencedObject()); }

			virtual Type GetType() const override { return PageNode::Type::PAGE_TREE_NODE; }

		private:
			ObjectReferenceWrapper<IntegerObject> _count;
			ObjectReferenceWrapper<SpecializedArrayObject<IndirectObjectReference>> _kids;
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
			ObjectReferenceWrapper<ArrayObject> _media_box;
		};

		class PageTree : public boost::intrusive_ref_counter<PageTree>
		{
		public:
			//PageTree();
			explicit PageTree(const IndirectObject& root);

			inline IntegerObject::ValueType PageCount(void) const { return _root->KidCount(); }
			inline ObjectReferenceWrapper<PageObject> Page(unsigned int number) const { return PageInternal(number); }
			inline ObjectReferenceWrapper<PageObject> operator[](unsigned int number) const { return PageInternal(number); }

			void Release();

		private:
			ObjectReferenceWrapper<PageObject> PageInternal(unsigned int number) const;

			ObjectReferenceWrapper<PageTreeNode> _root;
		};
	}
}

#endif /* _PAGE_TREE_H */
