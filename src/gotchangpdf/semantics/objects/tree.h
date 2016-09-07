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
			TreeBase(const syntax::DictionaryObjectPtr& obj)
				: HighLevelObject(obj), _root(this, obj) {}

			bool IsInitialized() const { return m_initialized; }

			void Initialize() const
			{
				if (m_initialized) {
					return;
				}

				m_map = GetAllKeys(_root);
				m_initialized = true;
			}

			void RemoveAllChilds()
			{
				if (_root->HasKids()) {
					auto kids = _root->Kids();
					for (auto kid : *kids) {
						// TODO release kid obj from file
					}
				}

				if (_root->HasValues()) {
					bool removed = _obj->Remove(GetValueName());
					assert(removed);
				}
			}

			const int LEAF_NODE_SIZE = 64;
			void Rebuild()
			{
				Initialize();

				// Remove all leaf and intermediate nodes
				RemoveAllChilds();

				syntax::MixedArrayObjectPtr leaf_values;
				leaf_values->SetFile(_obj->GetFile());
				leaf_values->SetInitialized();

				for (auto it = m_map.begin(); it != m_map.end(); ++it) {
					leaf_values->Append(it->first);
					leaf_values->Append(it->second);
				}

				//syntax::DictionaryObjectPtr leaf_dict;

				if (leaf_values->Size() > 0) {
					syntax::MixedArrayObjectPtr limit_values;
					auto lower_bound = leaf_values->At(0);
					auto upper_bound = leaf_values->At(leaf_values->Size() - 2);

					// Clone the raw values
					limit_values->Append(lower_bound->Clone());
					limit_values->Append(upper_bound->Clone());

					// Insert as first item into leaf dictionary
					//leaf_dict->Insert(constant::Name::Limits, limit_values);
					_obj->Insert(constant::Name::Limits, limit_values);
				}

				// Insert leaf values
				//leaf_dict->Insert(GetValueName(), leaf_values);
				_obj->Insert(GetValueName(), leaf_values);

				// Force tree reinitialization
				m_initialized = false;
				Initialize();
			}

		protected:

			bool Contains(const KeyT& key) const
			{
				Initialize();
				return (m_map.find(key) != m_map.end());
			}

			syntax::ContainableObjectPtr Find(const KeyT& key) const
			{
				Initialize();

				auto found = m_map.find(key);
				if (found == m_map.end()) {
					throw GeneralException("Item was not found");
				}

				return found->second;
			}

			void Insert(const KeyT& key, syntax::ContainableObjectPtr value)
			{
				Initialize();

				std::pair<KeyT, syntax::ContainableObjectPtr> pair(key, value);
				m_map.insert(pair);

				Rebuild();
			}

			bool Remove(const KeyT& key)
			{
				Initialize();

				bool erased = m_map.erase(key);
				bool result = (erased == m_map.end());
				Rebuild();

				return result;
			}

		private:
			class TreeNodeBase; using TreeNodeBasePtr = Deferred<TreeNodeBase>;
			class TreeNodeRoot; using TreeNodeRootPtr = Deferred<TreeNodeRoot>;
			class TreeNodeIntermediate; using TreeNodeIntermediatePtr = Deferred<TreeNodeIntermediate>;
			class TreeNodeLeaf; using TreeNodeLeafPtr = Deferred<TreeNodeLeaf>;

		private:
			TreeNodeRootPtr _root;
			mutable std::map<KeyT, syntax::ContainableObjectPtr> m_map;
			mutable bool m_initialized = false;

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

			void InsertPairsToMap(std::map<KeyT, syntax::ContainableObjectPtr>& map, const syntax::ArrayObjectPtr<syntax::ContainableObjectPtr>& values) const
			{
				int size = values->Size();
				for (int i = 0; i + 1 < size; i += 2) {
					auto key_obj = values->At(i);
					auto value_obj = values->At(i + 1);

					// TODO this assumes that key is derived from object
					auto key = syntax::ObjectUtils::ConvertTo<KeyT>(key_obj);
					auto value = value_obj;

					// Insert value
					std::pair<KeyT, syntax::ContainableObjectPtr> pair(key, value);
					map.insert(pair);
				}
			}

			std::map<KeyT, syntax::ContainableObjectPtr> GetAllKeys(const TreeNodeBasePtr& node) const
			{
				std::map<KeyT, syntax::ContainableObjectPtr> result_map;

				auto node_type = node->NodeType();
				if (node_type == TreeNodeBase::TreeNodeType::Root) {
					auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
					if (root->HasKids()) {
						auto kids = root->Kids();
						for (auto kid : *kids) {
							auto kid_map = GetAllKeys(kid);
							result_map.insert(kid_map.begin(), kid_map.end());
						}
					}

					if (root->HasValues()) {
						auto values = root->Values();
						InsertPairsToMap(result_map, values);
					}

					return result_map;
				}

				if (node_type == TreeNodeBase::TreeNodeType::Intermediate) {
					auto intermediate = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeIntermediatePtr>(node);
					auto kids = intermediate->Kids();
					for (auto kid : *kids) {
						auto kid_map = GetAllKeys(kid);
						result_map.insert(kid_map.begin(), kid_map.end());
					}

					return result_map;
				}

				if (node_type == TreeNodeBase::TreeNodeType::Leaf) {
					auto leaf = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeLeafPtr>(node);
					auto values = leaf->Values();

					InsertPairsToMap(result_map, values);
					return result_map;
				}

				throw GeneralException("Unknown node type");
			}

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
				auto node_type = node->NodeType();
				if (node_type == TreeNodeBase::TreeNodeType::Root) {
					auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
					if (root->HasKids()) {
						auto kids = root->Kids();

						bool contains = false;
						for (auto item : *kids) {
							contains |= ContainsInternal(item, key);
						}

						return contains;
					}

					if (root->HasValues()) {
						return ContainsInternal(root->Values(), key);
					}

					throw GeneralException("Unknown tree root type");
				}

				if (node_type == TreeNodeBase::TreeNodeType::Intermediate) {
					auto intermediate = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeIntermediatePtr>(node);
					return ContainsInternal(intermediate, key);
				}

				if (node_type == TreeNodeBase::TreeNodeType::Leaf) {
					auto leaf = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeLeafPtr>(node);
					auto limits = leaf->Limits();
					assert(limits->Size() == 2);
					if (limits->Size() != 2)
						return false;

					if (key < limits->At(0) || limits->At(1) < key)
						return false;

					return ContainsInternal(leaf->Values(), key);
				}

				throw GeneralException("Unknown node type");
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
				auto node_type = node->NodeType();
				if (node_type == TreeNodeBase::TreeNodeType::Root) {
					auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
					if (root->HasKids()) {
						auto kids = root->Kids();
						for (auto item : *kids) {
							if (ContainsInternal(item, key))
								return FindInternal(item, key);
						}

						throw GeneralException("Tree node does not contain required item");
					}

					if (root->HasValues()) {
						return FindInternal(root->Values(), key);
					}

					throw GeneralException("Unknown tree root type");
				}

				if (node_type == TreeNodeBase::TreeNodeType::Intermediate) {
					auto intermediate = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeIntermediatePtr>(node);
					for (auto item : *intermediate->Kids()) {
						if (ContainsInternal(item, key)) {
							return FindInternal(item, key);
						}
					}

					throw GeneralException("Tree node does not contain required item");
				}

				if (node_type == TreeNodeBase::TreeNodeType::Leaf) {
					auto leaf = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeLeafPtr>(node);
					auto limits = leaf->Limits();
					assert(limits->Size() == 2);
					if (limits->Size() != 2)
						throw GeneralException("Tree node has incorrect size");

					if (key < limits->At(0) || limits->At(1) < key)
						throw GeneralException("Tree node does not contain required item");

					return FindInternal(leaf->Values(), key);
				}

				throw GeneralException("Unknown node type");
			}
		};

		template <typename ValueT>
		class NameTree : public TreeBase<syntax::StringObjectPtr, ValueT>
		{
		public:
			using base_type = TreeBase<syntax::StringObjectPtr, ValueT>;

		public:
			NameTree(const syntax::DictionaryObjectPtr& obj, std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor)
				: TreeBase(obj), _conversion(convertor)
			{
			}

			virtual syntax::NameObjectPtr GetValueName(void) const override { return constant::Name::Names; }

			bool Contains(const syntax::StringObjectPtr& key) const
			{
				return base_type::Contains(key);
			}

			ValueT Find(const syntax::StringObjectPtr& key) const
			{
				return _conversion(base_type::Find(key));
			}

			void Insert(const syntax::StringObjectPtr& key, ValueT value)
			{
				auto raw_object = value->GetObject();

				syntax::IndirectObjectReferencePtr reference(raw_object);
				base_type::Insert(key, reference);
			}

			bool Remove(const syntax::StringObjectPtr& key)
			{
				return base_type::Remove(key);
			}

		private:
			std::function<ValueT(const syntax::ContainableObjectPtr&)> _conversion;
		};

		template <typename ValueT>
		class NumberTree : public TreeBase<syntax::IntegerObjectPtr, ValueT>
		{
		public:
			using base_type = TreeBase<syntax::IntegerObjectPtr, ValueT>;

		public:
			NumberTree(const syntax::DictionaryObjectPtr& obj, std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor)
				: TreeBase(obj), _conversion(convertor)
			{
			}

			virtual syntax::NameObjectPtr GetValueName(void) const override { return constant::Name::Nums; }

			bool Contains(const syntax::IntegerObjectPtr& key) const
			{
				return base_type::Contains(key);
			}

			ValueT Find(const syntax::IntegerObjectPtr& key) const
			{
				return _conversion(base_type::Find(key));
			}

			void Insert(const syntax::IntegerObjectPtr& key, ValueT value)
			{
				auto raw_object = value->GetObject();

				syntax::IndirectObjectReferencePtr reference(raw_object);
				base_type::Insert(key, reference);
			}

			bool Remove(const syntax::IntegerObjectPtr& key)
			{
				return base_type::Remove(key);
			}

		private:
			std::function<ValueT(const syntax::ContainableObjectPtr&)> _conversion;
		};
	}
}

#endif /* _TREE_H */
