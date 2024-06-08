#include "precompiled.h"
#include "semantics/objects/page_tree.h"

#include "syntax/files/file.h"
#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

using namespace syntax;

PageTree::PageTree(DictionaryObjectPtr root) : HighLevelObject(root) {
}

types::size_type PageTree::PageCount(void) const {
	auto root = make_deferred<PageTreeNode>(_obj);
	return root->KidCount()->SafeConvert<types::size_type>();
}

types::size_type PageTree::PageCount(PageNodeBasePtr node) {
	if (node->GetNodeType() == PageNodeBase::NodeType::Tree) {
		auto tree_node = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(node);
		auto result_obj = tree_node->KidCount();
		auto result_value = result_obj->SafeConvert<types::size_type>();

		types::size_type verify = 0;
		auto kids = tree_node->Kids();
		for (auto kid : kids) {
			verify += PageCount(kid);
		}

		assert(result_value == verify && "Kid count does not match"); UNUSED(verify);
		return result_value;
	}

	// Single page for objects
	if (node->GetNodeType() == PageNodeBase::NodeType::Object) {
		return 1;
	}

	throw GeneralException("Unknown page object type");
}

PageObjectPtr PageTree::GetCachedPage(types::size_type page_number) const {
	auto weak_file = GetObject()->GetFile();
	auto file = weak_file.GetReference();
	auto log_scope = file->GetFilenameString();

	LOG_DEBUG(log_scope) << "Searching for page " << std::to_string(page_number);

	if (page_number < 1) {
		throw GeneralException("Invalid page number: " + std::to_string(page_number));
	}

	auto root = make_deferred<PageTreeNode>(_obj);
	types::size_type pages_processed = 1;

	return PageInternal(root, page_number, pages_processed);
}

PageObjectPtr PageTree::PageInternal(PageTreeNodePtr node, types::size_type page_number, types::size_type& processed) const {
	auto kids = node->Kids();
	auto count = kids->GetSize();
	for (decltype(count) i = 0; i < count; ++i) {
		auto kid = kids->GetValue(i);

		if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
			auto tree_node = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(kid);
			auto kid_count = tree_node->KidCount()->SafeConvert<types::size_type>();
			if (processed + kid_count <= page_number) {
				processed += kid_count;
				continue;
			}

			if (HasTreeChilds(tree_node)) {
				return PageInternal(tree_node, page_number, processed);
			}

			auto result = tree_node->Kids()->GetValue(page_number - processed);
			auto page_object = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(result);
			return page_object;
		}

		if (kid->GetNodeType() == PageNodeBase::NodeType::Object) {

			assert(processed <= page_number && "Current page shall never reach above the required page number");
			if (processed != page_number) {
				processed++;
				continue;
			}

			auto page_object = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(kid);
			return page_object;
		}
	}

	throw GeneralException("Page number was not found: " + std::to_string(page_number));
}

bool PageTree::HasTreeChilds(PageTreeNodePtr node) const {
	auto kids = node->Kids();
	auto count = kids->GetSize();
	for (decltype(count) i = 0; i < count; ++i) {
		auto kid = kids->GetValue(i);
		if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
			return true;
		}
	}

	return false;
}

void PageTree::Insert(PageObjectPtr object, types::size_type page_index) {
	auto array_index = page_index - 1;

	auto raw_obj = object->GetObject();
	auto kids = GetKidsInternal();
	kids->Insert(array_index, make_deferred<IndirectReferenceObject>(raw_obj));
	object->SetParent(make_deferred<PageTreeNode>(_obj));

	UpdateKidsCount();
}

void PageTree::Append(PageObjectPtr object) {

	auto kids = GetKidsInternal();

	// Insert at the end of kids array
	Insert(object, kids->GetSize() + 1);
}

void PageTree::Remove(types::size_type page_index) {
	auto array_index = page_index - 1;

	auto kids = GetKidsInternal();
	bool removed = kids->Remove(array_index);
	assert(removed && "Could not remove page"); UNUSED(removed);

	UpdateKidsCount();
}

void PageTree::UpdateKidsCount() {
	auto root = make_deferred<PageTreeNode>(_obj);
	UpdateKidsCount(root);
}

types::size_type PageTree::UpdateKidsCount(PageNodeBasePtr node) {

	// Do nothing for page objects
	if (node->GetNodeType() == PageNodeBase::NodeType::Object) {
		return 1;
	}

	if (node->GetNodeType() == PageNodeBase::NodeType::Tree) {
		auto tree_node = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(node);
		auto tree_node_object = tree_node->GetObject();
		auto kid_count_object = tree_node_object->FindAs<IntegerObjectPtr>(constant::Name::Count);

		types::size_type kid_count = 0;
		auto kids = tree_node->Kids();
		for (auto kid : kids) {
			kid_count += UpdateKidsCount(kid);
		}

		// Update the kid count
		kid_count_object->SetValue(kid_count);
		return kid_count;
	}

	throw GeneralException("Unknown page object type");
}

ArrayObjectPtr<IndirectReferenceObjectPtr> PageTree::GetKidsInternal() {

	if (!_obj->Contains(constant::Name::Kids)) {
		ArrayObjectPtr<IndirectReferenceObjectPtr> empty_kids_obj;
		_obj->Insert(constant::Name::Kids, empty_kids_obj);
	}

	return _obj->FindAs<ArrayObjectPtr<IndirectReferenceObjectPtr>>(constant::Name::Kids);
}

// NOTE:
// The page tree cache was already tried multiple times and it was actually slower.
// I have seen this in the profiler, thus was trying to address, however if the cache does not make it faster
// there is probably another issues, that is not visible right now.
// Since there are more prevalent performance botllenecks I am dropping this for now.

#ifdef USE_PAGE_TREE_CACHE

void PageTree::ObserveeChanged(const IModifyObservable*) {
	// Reset the page object cache, so it has to be reconstructed
	m_page_object_cache.reset();
}

std::vector<PageObjectPtr> PageTree::InitializePageCache() const {

	std::vector<PageObjectPtr> new_pages;

	auto root = make_deferred<PageTreeNode>(_obj);
	InitializePageCacheInternal(root, new_pages);

	return new_pages;
}

void PageTree::InitializePageCacheInternal(PageTreeNodePtr node, std::vector<PageObjectPtr>& current_list) const {
	auto kids = node->Kids();
	auto count = kids->GetSize();
	for (decltype(count) i = 0; i < count; ++i) {
		auto kid = kids->GetValue(i);

		if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
			auto tree_node = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(kid);
			InitializePageCacheInternal(tree_node, current_list);
		}

		if (kid->GetNodeType() == PageNodeBase::NodeType::Object) {
			auto page_object = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(kid);
			current_list.push_back(page_object);
		}
	}
}

#endif /* USE_PAGE_TREE_CACHE */

} // semantics
} // vanillapdf
