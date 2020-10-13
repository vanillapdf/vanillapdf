#include "precompiled.h"
#include "semantics/objects/page_tree.h"

#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

using namespace syntax;

PageTree::PageTree(DictionaryObjectPtr root) : HighLevelObject(root) {
	auto page_count = PageCount();
	m_pages.resize(page_count);
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

		assert(result_value == verify && "Kid count does not match");
		return result_value;
	}

	// Single page for objects
	if (node->GetNodeType() == PageNodeBase::NodeType::Object) {
		return 1;
	}

	throw GeneralException("Unknown page object type");
}

PageObjectPtr PageTree::GetCachedPage(types::size_type page_number) const {
	if (page_number < 1) {
		throw GeneralException("Invalid page number: " + page_number);
	}

	auto page_size = m_pages.size();
	auto page_number_converted = ValueConvertUtils::SafeConvert<decltype(page_size)>(page_number);
	if (page_number_converted <= page_size) {
		auto& found_page = m_pages.at(page_number - 1);
		if (!found_page.empty()) {
			return found_page;
		}
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
			m_pages[page_number - 1] = page_object;
			return page_object;
		}

		if (kid->GetNodeType() == PageNodeBase::NodeType::Object) {

			assert(processed <= page_number && "Current page shall never reach above the required page number");
			if (processed != page_number) {
				processed++;
				continue;
			}

			auto page_object = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(kid);
			m_pages[page_number - 1] = page_object;
			return page_object;
		}
	}

	throw GeneralException("Page number was not found: " + page_number);
}

bool PageTree::HasTreeChilds(PageTreeNodePtr node) const {
	auto kids = node->Kids();
	auto count = kids->GetSize();;
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
	auto kids = _obj->FindAs<ArrayObjectPtr<IndirectReferenceObjectPtr>>(constant::Name::Kids);
	kids->Insert(array_index, make_deferred<IndirectReferenceObject>(raw_obj));
	object->SetParent(make_deferred<PageTreeNode>(_obj));

	UpdateKidsCount();
	m_pages.insert(m_pages.begin() + array_index, make_output<PageObjectPtr>(object));
}

void PageTree::Append(PageObjectPtr object) {
	auto kids = _obj->FindAs<ArrayObjectPtr<IndirectReferenceObjectPtr>>(constant::Name::Kids);

	// Insert at the end of kids array
	Insert(object, kids->GetSize() + 1);
}

void PageTree::Remove(types::size_type page_index) {
	auto array_index = page_index - 1;

	auto kids = _obj->FindAs<ArrayObjectPtr<IndirectReferenceObjectPtr>>(constant::Name::Kids);
	bool removed = kids->Remove(array_index);
	assert(removed && "Could not remove page"); UNUSED(removed);

	UpdateKidsCount();
	m_pages.erase(m_pages.begin() + array_index);
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

} // semantics
} // vanillapdf
