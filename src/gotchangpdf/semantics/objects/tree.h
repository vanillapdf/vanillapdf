#ifndef _TREE_H
#define _TREE_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "dictionary_object.h"
#include "string_object.h"
#include "containable.h"

#include "name_object.h"
#include "array_object.h"
#include "object_utils.h"
#include "util.h"

namespace gotchangpdf
{
	namespace semantics
	{
		template <typename KeyT, typename ValueT>
		class TreeBase : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			TreeBase(const syntax::DictionaryObjectPtr& obj, std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor)
				: HighLevelObject(obj), _root(this, obj), _conversion(convertor) {}

			bool Contains(const KeyT& key) const { return ContainsInternal(_root, key); }
			ValueT Find(const KeyT& key) const { return FindInternal(_root, key); }

		private:
			class TreeNodeBase; using TreeNodeBasePtr = Deferred<TreeNodeBase>;
			class TreeNodeRoot; using TreeNodeRootPtr = Deferred<TreeNodeRoot>;
			class TreeNodeIntermediate; using TreeNodeIntermediatePtr = Deferred<TreeNodeIntermediate>;
			class TreeNodeLeaf; using TreeNodeLeafPtr = Deferred<TreeNodeLeaf>;

		private:
			TreeNodeRootPtr _root;
			std::function<ValueT(const syntax::ContainableObjectPtr&)> _conversion;

			virtual syntax::NameObjectPtr GetValueName(void) const = 0;

		private:
			class TreeNodeBase : public HighLevelObject<syntax::DictionaryObjectPtr>
			{
			public:
				enum class TreeNodeType
				{
					Root,
					Intermediate,
					Leaf
				};

				using ParentT = TreeBase<KeyT, ValueT>;

				explicit TreeNodeBase(const ParentT* parent, const syntax::DictionaryObjectPtr& obj) : HighLevelObject(obj), _parent(parent) {}
				virtual ~TreeNodeBase() {}
				virtual TreeNodeType NodeType(void) const noexcept = 0;
				static TreeNodeBasePtr Create(const ParentT* parent, const syntax::DictionaryObjectPtr& obj)
				{
					bool kids = obj->Contains(constant::Name::Kids);
					bool limits = obj->Contains(constant::Name::Limits);
					bool names = obj->Contains(constant::Name::Names);

					if (kids && !limits && !names)
						return TreeNodeRootPtr(parent, obj);

					if (kids && limits && !names)
						return TreeNodeIntermediatePtr(parent, obj);

					if (!kids && limits && names)
						return TreeNodeLeafPtr(parent, obj);

					throw GeneralException("Unknown tree node: " + obj->ToString());
				}

			protected:
				const ParentT* _parent;
			};

			class TreeNodeRoot : public TreeNodeBase
			{
			public:
				explicit TreeNodeRoot(const ParentT* parent, const syntax::DictionaryObjectPtr& obj) : TreeNodeBase(parent, obj) {}
				virtual TreeNodeType NodeType(void) const noexcept override { return TreeNodeType::Root; }

				bool HasValues(void) const { return _obj->Contains(_parent->GetValueName()); }
				bool HasKids(void) const { return _obj->Contains(constant::Name::Kids); }
				syntax::ArrayObjectPtr<TreeNodeBasePtr> Kids(void) const
				{
					auto kids = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Kids);
					return kids->Convert<TreeNodeBasePtr>([this](const syntax::DictionaryObjectPtr& obj) { return TreeNodeBase::Create(_parent, obj); });
				}

				syntax::ArrayObjectPtr<syntax::ContainableObjectPtr> Values(void) const { return _obj->FindAs<syntax::ArrayObjectPtr<syntax::ContainableObjectPtr>>(_parent->GetValueName()); }
			};

			class TreeNodeIntermediate : public TreeNodeBase
			{
			public:
				explicit TreeNodeIntermediate(const ParentT* parent, const syntax::DictionaryObjectPtr& obj) : TreeNodeBase(parent, obj) {}
				virtual TreeNodeType NodeType(void) const noexcept override { return TreeNodeType::Intermediate; }

				syntax::ArrayObjectPtr<TreeNodeBasePtr> Kids(void) const
				{
					auto kids = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Kids);
					return kids->Convert<TreeNodeBasePtr>([this](const syntax::DictionaryObjectPtr& obj) { return TreeNodeBase::Create(_parent, obj); });
				}

				syntax::ArrayObjectPtr<KeyT> Limits(void) const { return _obj->FindAs<syntax::ArrayObjectPtr<KeyT>>(constant::Name::Limits); }
			};

			class TreeNodeLeaf : public TreeNodeBase
			{
			public:
				explicit TreeNodeLeaf(const ParentT* parent, const syntax::DictionaryObjectPtr& obj) : TreeNodeBase(parent, obj) {}
				virtual TreeNodeType NodeType(void) const noexcept override { return TreeNodeType::Leaf; }

				syntax::ArrayObjectPtr<KeyT> Limits(void) const { return _obj->FindAs<syntax::ArrayObjectPtr<KeyT>>(constant::Name::Limits); }
				syntax::ArrayObjectPtr<syntax::ContainableObjectPtr> Values(void) const { return _obj->FindAs<syntax::ArrayObjectPtr<syntax::ContainableObjectPtr>>(_parent->GetValueName()); }
			};

		private:
			bool ContainsInternal(const syntax::ArrayObjectPtr<syntax::ContainableObjectPtr>& values, const KeyT& key) const
			{
				int size = values->Size();
				for (int i = 0; i + 1 < size; i += 2) {
					if (syntax::ObjectUtils::ValueEquals(values->At(i), key)) {
						return true;
					}
				}

				return false;
			}

			bool ContainsInternal(const TreeNodeBasePtr& node, const KeyT& key) const
			{
				switch (node->NodeType()) {
				case TreeNodeBase::TreeNodeType::Root:
				{
					auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
					if (root->HasKids()) {
						for (auto item : *root->Kids()) {
							if (ContainsInternal(item, key))
								return true;
						}
					}

					if (root->HasValues()) {
						return ContainsInternal(root->Values(), key);
					}

					throw GeneralException("Unknown tree root type");
				} break;
				case TreeNodeBase::TreeNodeType::Intermediate:
				{
					auto intermediate = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeIntermediatePtr>(node);
					if (ContainsInternal(intermediate, key)) {
						return true;
					}
				} break;
				case TreeNodeBase::TreeNodeType::Leaf:
				{
					auto leaf = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeLeafPtr>(node);
					auto limits = leaf->Limits();
					assert(limits->Size() == 2);
					if (limits->Size() != 2)
						return false;

					if (key < limits->At(0) || limits->At(1) < key)
						return false;

					return ContainsInternal(leaf->Values(), key);
				} break;
				default:
					assert(!"Unknown node type");
				}

				return false;
			}

			ValueT FindInternal(const syntax::ArrayObjectPtr<syntax::ContainableObjectPtr>& values, const KeyT& key) const
			{
				int size = values->Size();
				for (int i = 0; i + 1 < size; i += 2) {
					if (syntax::ObjectUtils::ValueEquals(values->At(i), key)) {
						return _conversion(values->At(i + 1));
					}
				}

				throw GeneralException("Tree node does not contain required item");
			}

			ValueT FindInternal(const TreeNodeBasePtr& node, const KeyT& key) const
			{
				switch (node->NodeType()) {
				case TreeNodeBase::TreeNodeType::Root:
				{
					auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
					if (root->HasKids()) {
						for (auto item : *root->Kids()) {
							if (ContainsInternal(item, key))
								return FindInternal(item, key);
						}
					}

					if (root->HasValues()) {
						return FindInternal(root->Values(), key);
					}

					throw GeneralException("Unknown tree root type");
				} break;
				case TreeNodeBase::TreeNodeType::Intermediate:
				{
					auto intermediate = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeIntermediatePtr>(node);
					for (auto item : *intermediate->Kids()) {
						if (ContainsInternal(item, key)) {
							return FindInternal(item, key);
						}
					}
				} break;
				case TreeNodeBase::TreeNodeType::Leaf:
				{
					auto leaf = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeLeafPtr>(node);
					auto limits = leaf->Limits();
					assert(limits->Size() == 2);
					if (limits->Size() != 2)
						throw GeneralException("Tree node has incorrect size");

					if (key < limits->At(0) || limits->At(1) < key)
						throw GeneralException("Tree node does not contain required item");

					return FindInternal(leaf->Values(), key);
				} break;
				default:
					assert(!"Unknown node type");
				}

				throw GeneralException("Tree node does not contain required item");
			}
		};

		template <typename ValueT>
		class NameTree : public TreeBase<syntax::StringObjectPtr, ValueT>
		{
		public:
			NameTree(const syntax::DictionaryObjectPtr& obj, std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor) : TreeBase(obj, convertor) {}
			virtual syntax::NameObjectPtr GetValueName(void) const override { return constant::Name::Names; }
		};

		template <typename ValueT>
		class NumberTree : public TreeBase<syntax::IntegerObjectPtr, ValueT>
		{
		public:
			NumberTree(const syntax::DictionaryObjectPtr& obj, std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor) : TreeBase(obj, convertor) {}
			virtual syntax::NameObjectPtr GetValueName(void) const override { return constant::Name::Nums; }
		};
	}
}

#endif /* _TREE_H */
