#ifndef _TREE_H
#define _TREE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/files/xref.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/string_object.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"

#include "utils/util.h"

namespace vanillapdf {
namespace semantics {

class IValueNameProvider {
public:
	virtual const syntax::NameObject& GetValueName(void) const = 0;
	virtual ~IValueNameProvider() {}
};

class TreeNodeBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	enum class TreeNodeType {
		Root,
		Intermediate,
		Leaf
	};

	explicit TreeNodeBase(const IValueNameProvider* parent, syntax::DictionaryObjectPtr obj);
	virtual TreeNodeType NodeType(void) const noexcept = 0;
	static TreeNodeBasePtr Create(const IValueNameProvider* parent, syntax::DictionaryObjectPtr obj);

protected:
	const IValueNameProvider* _parent;
};

class TreeNodeRoot : public TreeNodeBase {
public:
	explicit TreeNodeRoot(const IValueNameProvider* parent, syntax::DictionaryObjectPtr obj);
	virtual TreeNodeType NodeType(void) const noexcept override;

	bool HasValues(void) const;
	bool HasKids(void) const;
	syntax::ArrayObjectPtr<TreeNodeBasePtr> Kids(void) const;
	syntax::MixedArrayObjectPtr Values(void) const;
};

class TreeNodeIntermediate : public TreeNodeBase {
public:
	explicit TreeNodeIntermediate(const IValueNameProvider* parent, const syntax::DictionaryObjectPtr& obj);
	virtual TreeNodeType NodeType(void) const noexcept override;

	syntax::ArrayObjectPtr<TreeNodeBasePtr> Kids(void) const;
	syntax::MixedArrayObjectPtr Limits(void) const;
};

class TreeNodeLeaf : public TreeNodeBase {
public:
	explicit TreeNodeLeaf(const IValueNameProvider* parent, const syntax::DictionaryObjectPtr& obj);
	virtual TreeNodeType NodeType(void) const noexcept override;

	syntax::MixedArrayObjectPtr Limits(void) const;
	syntax::MixedArrayObjectPtr Values(void) const;
};

template <typename KeyT, typename ValueT>
class TreeBase : public HighLevelObject<syntax::DictionaryObjectPtr>, public IValueNameProvider {
public:
	using map_type = std::map<KeyT, syntax::ContainableObjectPtr>;

	typedef typename map_type::value_type value_type;
	typedef typename map_type::iterator iterator;
	typedef typename map_type::const_iterator const_iterator;
	typedef typename map_type::size_type size_type;
	typedef typename map_type::reference reference;
	typedef typename map_type::const_reference const_reference;
	typedef typename map_type::difference_type difference_type;

public:
	explicit TreeBase(syntax::DictionaryObjectPtr obj);
	bool IsInitialized() const;
	void Initialize() const;
	void RemoveAllChilds();
	void ReleaseNode(TreeNodeBasePtr node);
	void Rebuild();

protected:
	bool Contains(const KeyT& key) const;
	syntax::ContainableObjectPtr Find(const KeyT& key) const;
	void Insert(const KeyT& key, syntax::ContainableObjectPtr value);
	bool Remove(const KeyT& key);

	// stl compatibility
	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

private:
	TreeNodeRootPtr _root;
	mutable map_type m_map;
	mutable bool m_initialized = false;

	void InsertPairsToMap(std::map<KeyT, syntax::ContainableObjectPtr>& map, const syntax::MixedArrayObjectPtr values) const;
	std::map<KeyT, syntax::ContainableObjectPtr> GetAllKeys(const TreeNodeBasePtr node) const;
	bool ContainsInternal(const syntax::MixedArrayObjectPtr values, const KeyT& key) const;
	bool ContainsInternal(const TreeNodeBasePtr node, const KeyT& key) const;
	ValueT FindInternal(const syntax::MixedArrayObjectPtr values, const KeyT& key) const;
	ValueT FindInternal(const TreeNodeBasePtr node, const KeyT& key) const;
};

template <typename ValueT>
class NameTree : public TreeBase<syntax::StringObjectPtr, ValueT> {
public:
	using base_type = TreeBase<syntax::StringObjectPtr, ValueT>;
	using conversion_type = std::function<ValueT(const syntax::ContainableObjectPtr&)>;

	class Iterator : public BaseIterator<typename base_type::map_type::const_iterator> {
	public:
		using base_iterator = typename base_type::map_type::const_iterator;
		using base_iterator_type = BaseIterator<base_iterator>;

	public:
		Iterator(base_iterator current, base_iterator invalid, conversion_type convertor)
			: base_iterator_type(current, invalid), _conversion(convertor) {
		}

		const Iterator& operator++() {
			++base_iterator_type::m_current;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(base_iterator_type::m_current, base_iterator_type::m_invalid);
			++base_iterator_type::m_current;
			return temp;
		}

		std::pair<syntax::StringObjectPtr, ValueT> operator*() {
			auto first = base_iterator_type::m_current->first;
			auto second = _conversion(base_iterator_type::m_current->second);
			return std::pair<syntax::StringObjectPtr, ValueT>(first, second);
		}

	private:
		std::function<ValueT(const syntax::ContainableObjectPtr&)> _conversion;
	};

public:
	typedef ValueT value_type;
	typedef Iterator iterator;
	typedef Iterator const_iterator;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

public:
	NameTree(syntax::DictionaryObjectPtr obj,
		std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor);

	virtual const syntax::NameObject& GetValueName(void) const override;
	bool Contains(const syntax::StringObjectPtr& key) const;
	ValueT Find(const syntax::StringObjectPtr& key) const;
	void Insert(const syntax::StringObjectPtr& key, ValueT value);
	bool Remove(const syntax::StringObjectPtr& key);

	// stl compatibility
	iterator begin() {
		return Iterator(base_type::begin(), base_type::end(), _conversion);
	}

	const_iterator begin() const {
		return Iterator(base_type::begin(), base_type::end(), _conversion);
	}

	iterator end() {
		return Iterator(base_type::end(), base_type::end(), _conversion);
	}

	const_iterator end() const {
		return Iterator(base_type::end(), base_type::end(), _conversion);
	}

private:
	std::function<ValueT(const syntax::ContainableObjectPtr&)> _conversion;
};

template <typename ValueT>
class NumberTree : public TreeBase<syntax::IntegerObjectPtr, ValueT> {
public:
	using base_type = TreeBase<syntax::IntegerObjectPtr, ValueT>;

public:
	NumberTree(syntax::DictionaryObjectPtr obj,
		std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor
		);

	virtual const syntax::NameObject& GetValueName(void) const override;
	bool Contains(const syntax::IntegerObjectPtr& key) const;
	ValueT Find(const syntax::IntegerObjectPtr& key) const;
	void Insert(const syntax::IntegerObjectPtr& key, ValueT value);
	bool Remove(const syntax::IntegerObjectPtr& key);

private:
	std::function<ValueT(const syntax::ContainableObjectPtr&)> _conversion;
};

/****************************************************************************************************************************************/
/*														Definition starts here															*/
/****************************************************************************************************************************************/

#pragma region Tree base

template <typename KeyT, typename ValueT>
TreeBase<KeyT, ValueT>::TreeBase(syntax::DictionaryObjectPtr obj)
	: HighLevelObject(obj), _root(make_deferred<TreeNodeRoot>(this, obj)) {
}

template <typename KeyT, typename ValueT>
typename TreeBase<KeyT, ValueT>::iterator TreeBase<KeyT, ValueT>::begin() {
	Initialize();
	return m_map.begin();
}

template <typename KeyT, typename ValueT>
typename TreeBase<KeyT, ValueT>::const_iterator TreeBase<KeyT, ValueT>::begin() const {
	Initialize();
	return m_map.begin();
}

template <typename KeyT, typename ValueT>
typename TreeBase<KeyT, ValueT>::iterator TreeBase<KeyT, ValueT>::end() {
	Initialize();
	return m_map.end();
}

template <typename KeyT, typename ValueT>
typename TreeBase<KeyT, ValueT>::const_iterator TreeBase<KeyT, ValueT>::end() const {
	Initialize();
	return m_map.end();
}

template <typename KeyT, typename ValueT>
bool TreeBase<KeyT, ValueT>::IsInitialized() const {
	return m_initialized;
}

template <typename KeyT, typename ValueT>
void TreeBase<KeyT, ValueT>::Initialize() const {
	if (m_initialized) {
		return;
	}

	m_map = GetAllKeys(_root);
	m_initialized = true;
}

template <typename KeyT, typename ValueT>
void TreeBase<KeyT, ValueT>::ReleaseNode(TreeNodeBasePtr node) {
	if (node->NodeType() == TreeNodeBase::TreeNodeType::Leaf) {
		auto leaf = ConvertUtils<TreeNodeBasePtr>::ConvertTo <TreeNodeLeafPtr> (node);

		auto leaf_obj = leaf->GetObject();
		assert(leaf_obj->IsIndirect() && "Tree kids shall be indirect");

		auto weak_leaf_xref = leaf_obj->GetXrefEntry();
		if (weak_leaf_xref.IsActive() && !weak_leaf_xref.IsEmpty()) {
			auto leaf_xref = weak_leaf_xref.GetReference();
			leaf_xref->ReleaseReference(true);
		}
	}

	if (node->NodeType() == TreeNodeBase::TreeNodeType::Intermediate) {
		auto intermediate = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeIntermediatePtr>(node);

		auto kids = intermediate->Kids();
		for (auto kid : kids) {
			ReleaseNode(kid);
		}

		auto intermediate_obj = intermediate->GetObject();
		assert(intermediate_obj->IsIndirect() && "Tree kids shall be indirect");

		auto weak_intermediate_xref = intermediate_obj->GetXrefEntry();
		if (weak_intermediate_xref.IsActive() && !weak_intermediate_xref.IsEmpty()) {
			auto intermediate_xref = weak_intermediate_xref.GetReference();
			intermediate_xref->ReleaseReference(true);
		}
	}
}

template <typename KeyT, typename ValueT>
void TreeBase<KeyT, ValueT>::RemoveAllChilds() {
	if (_root->HasKids()) {
		auto kids = _root->Kids();
		for (auto kid : kids) {
			ReleaseNode(kid);
		}
	}

	if (_root->HasValues()) {
		bool removed = _obj->Remove(GetValueName());
		assert(removed); UNUSED(removed);
	}
}

template <typename KeyT, typename ValueT>
void TreeBase<KeyT, ValueT>::Rebuild() {
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

	if (leaf_values->GetSize() > 0) {
		syntax::MixedArrayObjectPtr limit_values;
		auto lower_bound = leaf_values->At(0);
		auto upper_bound = leaf_values->At(leaf_values->GetSize() - 2);

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

template <typename KeyT, typename ValueT>
bool TreeBase<KeyT, ValueT>::Contains(const KeyT& key) const {
	Initialize();
	return (m_map.find(key) != m_map.end());
}

template <typename KeyT, typename ValueT>
syntax::ContainableObjectPtr TreeBase<KeyT, ValueT>::Find(const KeyT& key) const {
	Initialize();

	auto found = m_map.find(key);
	if (found == m_map.end()) {
		throw GeneralException("Item was not found");
	}

	return found->second;
}

template <typename KeyT, typename ValueT>
void TreeBase<KeyT, ValueT>::Insert(const KeyT& key, syntax::ContainableObjectPtr value) {
	Initialize();

	std::pair<KeyT, syntax::ContainableObjectPtr> pair(key, value);
	m_map.insert(pair);

	Rebuild();
}

template <typename KeyT, typename ValueT>
bool TreeBase<KeyT, ValueT>::Remove(const KeyT& key) {
	Initialize();

	bool erased = m_map.erase(key);
	bool result = (erased == m_map.end());
	Rebuild();

	return result;
}

template <typename KeyT, typename ValueT>
void TreeBase<KeyT, ValueT>::InsertPairsToMap(std::map<KeyT, syntax::ContainableObjectPtr>& map, const syntax::MixedArrayObjectPtr values) const {
	auto size = values->GetSize();
	for (decltype(size) i = 0; i + 1 < size; i += 2) {
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

template <typename KeyT, typename ValueT>
std::map<KeyT, syntax::ContainableObjectPtr> TreeBase<KeyT, ValueT>::GetAllKeys(const TreeNodeBasePtr node) const {
	std::map<KeyT, syntax::ContainableObjectPtr> result_map;

	auto node_type = node->NodeType();
	if (node_type == TreeNodeBase::TreeNodeType::Root) {
		auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
		if (root->HasKids()) {
			auto kids = root->Kids();
			for (auto kid : kids) {
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
		for (auto kid : kids) {
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

template <typename KeyT, typename ValueT>
bool TreeBase<KeyT, ValueT>::ContainsInternal(const syntax::MixedArrayObjectPtr values, const KeyT& key) const {
	int size = values->GetSize();
	for (int i = 0; i + 1 < size; i += 2) {
		if (values->At(i)->Equals(key)) {
			return true;
		}
	}

	return false;
}

template <typename KeyT, typename ValueT>
bool TreeBase<KeyT, ValueT>::ContainsInternal(const TreeNodeBasePtr node, const KeyT& key) const {
	auto node_type = node->NodeType();
	if (node_type == TreeNodeBase::TreeNodeType::Root) {
		auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
		if (root->HasKids()) {
			auto kids = root->Kids();

			bool contains = false;
			for (auto item : kids) {
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
		assert(limits->GetSize() == 2);
		if (limits->GetSize() != 2)
			return false;

		if (key < limits->At(0) || limits->At(1) < key)
			return false;

		return ContainsInternal(leaf->Values(), key);
	}

	throw GeneralException("Unknown node type");
}

template <typename KeyT, typename ValueT>
ValueT TreeBase<KeyT, ValueT>::FindInternal(const syntax::MixedArrayObjectPtr values, const KeyT& key) const {
	int size = values->GetSize();
	for (int i = 0; i + 1 < size; i += 2) {
		if (values->At(i)->Equals(key)) {
			return values->At(i + 1);
		}
	}

	throw GeneralException("Tree node does not contain required item");
}

template <typename KeyT, typename ValueT>
ValueT TreeBase<KeyT, ValueT>::FindInternal(const TreeNodeBasePtr node, const KeyT& key) const {
	auto node_type = node->NodeType();
	if (node_type == TreeNodeBase::TreeNodeType::Root) {
		auto root = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeRootPtr>(node);
		if (root->HasKids()) {
			auto kids = root->Kids();
			for (auto item : kids) {
				if (ContainsInternal(item, key)) {
					return FindInternal(item, key);
				}
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
		for (auto item : intermediate->Kids()) {
			if (ContainsInternal(item, key)) {
				return FindInternal(item, key);
			}
		}

		throw GeneralException("Tree node does not contain required item");
	}

	if (node_type == TreeNodeBase::TreeNodeType::Leaf) {
		auto leaf = ConvertUtils<TreeNodeBasePtr>::ConvertTo<TreeNodeLeafPtr>(node);
		auto limits = leaf->Limits();
		assert(limits->GetSize() == 2);
		if (limits->GetSize() != 2)
			throw GeneralException("Tree node has incorrect size");

		if (key < limits->At(0) || limits->At(1) < key)
			throw GeneralException("Tree node does not contain required item");

		return FindInternal(leaf->Values(), key);
	}

	throw GeneralException("Unknown node type");
}

#pragma endregion

#pragma region Name Tree

template <typename ValueT>
inline NameTree<ValueT>::NameTree(
	syntax::DictionaryObjectPtr obj,
	std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor)
	: TreeBase<syntax::StringObjectPtr, ValueT>(obj), _conversion(convertor) {}

template <typename ValueT>
inline const syntax::NameObject& NameTree<ValueT>::GetValueName(void) const {
	return constant::Name::Names;
}

template <typename ValueT>
inline bool NameTree<ValueT>::Contains(const syntax::StringObjectPtr& key) const {
	return base_type::Contains(key);
}

template <typename ValueT>
inline ValueT NameTree<ValueT>::Find(const syntax::StringObjectPtr& key) const {
	return _conversion(base_type::Find(key));
}

template <typename ValueT>
inline void NameTree<ValueT>::Insert(const syntax::StringObjectPtr& key, ValueT value) {
	auto raw_object = value->GetObject();

	syntax::IndirectReferenceObjectPtr reference = make_deferred<syntax::IndirectReferenceObject>(raw_object);
	base_type::Insert(key, reference);
}

template <typename ValueT>
inline bool NameTree<ValueT>::Remove(const syntax::StringObjectPtr& key) {
	return base_type::Remove(key);
}

#pragma endregion

#pragma region Number Tree

template <typename ValueT>
inline NumberTree<ValueT>::NumberTree(
	syntax::DictionaryObjectPtr obj,
	std::function<ValueT(const syntax::ContainableObjectPtr&)> convertor)
	: TreeBase<syntax::IntegerObjectPtr, ValueT>(obj), _conversion(convertor) {}

template <typename ValueT>
inline const syntax::NameObject& NumberTree<ValueT>::GetValueName(void) const {
	return constant::Name::Nums;
}

template <typename ValueT>
inline bool NumberTree<ValueT>::Contains(const syntax::IntegerObjectPtr& key) const {
	return base_type::Contains(key);
}

template <typename ValueT>
inline ValueT NumberTree<ValueT>::Find(const syntax::IntegerObjectPtr& key) const {
	return _conversion(base_type::Find(key));
}

template <typename ValueT>
inline void NumberTree<ValueT>::Insert(const syntax::IntegerObjectPtr& key, ValueT value) {
	auto raw_object = value->GetObject();

	syntax::IndirectReferenceObjectPtr reference(raw_object);
	base_type::Insert(key, reference);
}

template <typename ValueT>
inline bool NumberTree<ValueT>::Remove(const syntax::IntegerObjectPtr& key) {
	return base_type::Remove(key);
}

#pragma endregion

} // semantics
} // vanillapdf

#endif /* _TREE_H */
