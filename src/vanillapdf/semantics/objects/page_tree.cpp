#include "precompiled.h"
#include "semantics/objects/page_tree.h"

#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include <fmt/core.h>

namespace vanillapdf {
namespace semantics {

using namespace syntax;

PageTree::PageTree(DictionaryObjectPtr root) : HighLevelObject(root) {
    m_cache_lock = std::unique_ptr<std::recursive_mutex>(pdf_new std::recursive_mutex());
}

types::size_type PageTree::PageCount(void) const {
    auto root = make_deferred<PageTreeNode>(_obj);
    return root->KidCount()->SafeConvert<types::size_type>();
}


void PageTree::BuildPageCache() const {
    auto root = make_deferred<PageTreeNode>(_obj);
    auto root_kids = root->Kids();
    auto root_count = root_kids->GetSize();

    // Seed the stack with root's children in reverse so the first child is on top.
    std::vector<PageNodeBasePtr> stack;
    for (auto i = root_count; i > 0; i -= 1) {
        stack.push_back(root_kids->GetValue(i - 1));
    }

    while (!stack.empty()) {
        auto node = stack.back();
        stack.pop_back();

        if (node->GetNodeType() == PageNodeBase::NodeType::Tree) {
            auto tree = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(node);
            auto kids = tree->Kids();
            auto count = kids->GetSize();
            for (auto i = count; i > 0; i -= 1) {
                stack.push_back(kids->GetValue(i - 1));
            }
            continue;
        }

        auto page = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(node);
        m_page_cache.push_back(page->GetObject());
    }

    m_cache_built = true;
}

PageObjectPtr PageTree::GetCachedPage(types::size_type page_number) const {
    if (page_number < 1) {
        throw InvalidParameterException(fmt::format("Invalid page number: {}", page_number));
    }

    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!m_cache_built) {
        BuildPageCache();
    }

    if (page_number > m_page_cache.size()) {
        throw ObjectMissingException("Page number was not found: " + std::to_string(page_number));
    }

    spdlog::debug("Searching for page {}", page_number);

    return make_deferred<PageObject>(m_page_cache[page_number - 1]);
}

void PageTree::WarmPageCache() const {
    ACCESS_LOCK_GUARD(m_cache_lock);

    if (!m_cache_built) {
        BuildPageCache();
    }
}

void PageTree::InvalidatePageCache() {
    ACCESS_LOCK_GUARD(m_cache_lock);

    m_page_cache.clear();
    m_cache_built = false;
}


bool PageTree::FindPageIndex(DictionaryObjectPtr page_dict, types::size_type& result) const {
    auto root = make_deferred<PageTreeNode>(_obj);
    types::size_type current_index = 1;
    if (FindPageIndexInternal(root, page_dict, current_index)) {
        result = current_index;
        return true;
    }
    return false;
}

bool PageTree::FindPageIndexInternal(PageTreeNodePtr node, DictionaryObjectPtr page_dict, types::size_type& current_index) const {
    auto kids = node->Kids();
    auto count = kids->GetSize();
    for (decltype(count) i = 0; i < count; ++i) {
        auto kid = kids->GetValue(i);

        if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
            auto tree_node = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(kid);
            if (FindPageIndexInternal(tree_node, page_dict, current_index)) {
                return true;
            }
            continue;
        }

        if (kid->GetNodeType() == PageNodeBase::NodeType::Object) {
            auto page_object = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(kid);
            if (page_object->GetObject()->Identity(page_dict)) {
                return true;
            }
            current_index++;
        }
    }
    return false;
}


PageTreeNodePtr PageTree::FindPageParent(types::size_type page_number, types::size_type& kid_index) const {
    auto root = make_deferred<PageTreeNode>(_obj);

    // The result placeholder is overwritten on success and unused on failure
    auto parent_node = root;

    types::size_type current_page_number = 0;
    if (!FindPageParentInternal(root, page_number, current_page_number, parent_node, kid_index)) {
        LOG_ERROR_AND_THROW(ObjectMissingException, "Page number was not found: {}", page_number);
    }

    return parent_node;
}

bool PageTree::FindPageParentInternal(PageTreeNodePtr node, types::size_type page_number, types::size_type& current_page_number, PageTreeNodePtr& parent_node, types::size_type& kid_index) const {
    auto kids = node->Kids();
    auto count = kids->GetSize();
    for (decltype(count) i = 0; i < count; ++i) {
        auto kid = kids->GetValue(i);

        if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
            auto tree_node = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(kid);
            if (FindPageParentInternal(tree_node, page_number, current_page_number, parent_node, kid_index)) {
                return true;
            }
            continue;
        }

        current_page_number += 1;
        if (current_page_number == page_number) {
            parent_node = node;
            kid_index = i;
            return true;
        }
    }

    return false;
}

void PageTree::Insert(PageObjectPtr object, types::size_type page_index) {
    if (page_index < 1) {
        throw InvalidParameterException(fmt::format("Invalid page index: {}. Page indices are 1-based", page_index));
    }

    // Inserting after the current last page is an append
    if (page_index == PageCount() + 1) {
        Append(object);
        return;
    }

    // The new page lands in the node that currently holds the page at the
    // target flat position, so it takes that page number. A page's Parent
    // entry must reference the node whose Kids array contains it.
    auto raw_obj = object->GetObject();
    auto page_reference = make_deferred<IndirectReferenceObject>(raw_obj);

    types::size_type kid_index = 0;
    auto parent_node = FindPageParent(page_index, kid_index);
    auto parent_kids = GetKidsInternal(parent_node->GetObject());
    parent_kids->Insert(kid_index, page_reference);
    object->SetParent(parent_node);

    UpdateKidsCount();
    InvalidatePageCache();
}

void PageTree::Append(PageObjectPtr object) {

    // Appended pages become direct children of the root node - the position
    // after the last page always coincides with the end of the root Kids
    // array, and without tree balancing there is no benefit in nesting deeper
    auto raw_obj = object->GetObject();
    auto page_reference = make_deferred<IndirectReferenceObject>(raw_obj);

    auto root_kids = GetKidsInternal(_obj);
    root_kids->Append(page_reference);
    object->SetParent(make_deferred<PageTreeNode>(_obj));

    UpdateKidsCount();
    InvalidatePageCache();
}

void PageTree::Remove(types::size_type page_index) {
    if (page_index < 1) {
        throw InvalidParameterException(fmt::format("Invalid page index: {}. Page indices are 1-based", page_index));
    }

    types::size_type kid_index = 0;
    auto parent_node = FindPageParent(page_index, kid_index);
    auto parent_dictionary = parent_node->GetObject();

    auto parent_kids = GetKidsInternal(parent_dictionary);
    bool removed = parent_kids->Remove(kid_index);
    if (!removed) {
        LOG_ERROR_AND_THROW_GENERAL("Could not remove page {} from its parent node", page_index);
    }

    // Prune intermediate nodes left empty by the removal, so the tree does
    // not accumulate childless Pages nodes. The root node always stays.
    auto current_dictionary = parent_dictionary;
    while (!current_dictionary->Identity(_obj) && GetKidsInternal(current_dictionary)->GetSize() == 0) {
        if (!current_dictionary->Contains(constant::Name::Parent)) {
            break;
        }

        auto node_parent_dictionary = current_dictionary->FindAs<DictionaryObjectPtr>(constant::Name::Parent);
        auto node_parent_kids = GetKidsInternal(node_parent_dictionary);

        bool pruned = false;
        auto node_parent_kids_size = node_parent_kids->GetSize();
        for (decltype(node_parent_kids_size) i = 0; i < node_parent_kids_size; ++i) {
            auto kid_reference = node_parent_kids->GetValue(i);
            if (kid_reference->GetReferencedObject()->Identity(current_dictionary)) {
                pruned = node_parent_kids->Remove(i);
                break;
            }
        }

        if (!pruned) {
            break;
        }

        current_dictionary = node_parent_dictionary;
    }

    UpdateKidsCount();
    InvalidatePageCache();
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

    throw syntax::ParseException("Unknown page object type");
}

ArrayObjectPtr<IndirectReferenceObjectPtr> PageTree::GetKidsInternal(DictionaryObjectPtr node_dictionary) {

    if (!node_dictionary->Contains(constant::Name::Kids)) {
        ArrayObjectPtr<IndirectReferenceObjectPtr> empty_kids_obj;
        node_dictionary->Insert(constant::Name::Kids, empty_kids_obj);
    }

    return node_dictionary->FindAs<ArrayObjectPtr<IndirectReferenceObjectPtr>>(constant::Name::Kids);
}

} // semantics
} // vanillapdf
