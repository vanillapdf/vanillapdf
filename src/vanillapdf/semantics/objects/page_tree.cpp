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
    m_page_cache.reserve(PageCount());
    CollectPageDicts(make_deferred<PageTreeNode>(_obj));
}

void PageTree::CollectPageDicts(PageTreeNodePtr node) const {
    auto kids = node->Kids();
    auto count = kids->GetSize();
    for (decltype(count) i = 0; i < count; i += 1) {
        auto kid = kids->GetValue(i);
        if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
            CollectPageDicts(ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(kid));
        } else {
            auto page = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(kid);
            m_page_cache.push_back(page->GetObject());
        }
    }
}

PageObjectPtr PageTree::GetCachedPage(types::size_type page_number) const {
    if (page_number < 1) {
        throw InvalidParameterException(fmt::format("Invalid page number: {}", page_number));
    }

    ACCESS_LOCK_GUARD(m_cache_lock);

    if (m_page_cache.empty()) {
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

    if (m_page_cache.empty()) {
        BuildPageCache();
    }
}

void PageTree::InvalidatePageCache() {
    ACCESS_LOCK_GUARD(m_cache_lock);

    m_page_cache.clear();
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


void PageTree::Insert(PageObjectPtr object, types::size_type page_index) {
    auto array_index = page_index - 1;

    auto raw_obj = object->GetObject();
    auto kids = GetKidsInternal();
    kids->Insert(array_index, make_deferred<IndirectReferenceObject>(raw_obj));
    object->SetParent(make_deferred<PageTreeNode>(_obj));

    UpdateKidsCount();
    InvalidatePageCache();
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

ArrayObjectPtr<IndirectReferenceObjectPtr> PageTree::GetKidsInternal() {

    if (!_obj->Contains(constant::Name::Kids)) {
        ArrayObjectPtr<IndirectReferenceObjectPtr> empty_kids_obj;
        _obj->Insert(constant::Name::Kids, empty_kids_obj);
    }

    return _obj->FindAs<ArrayObjectPtr<IndirectReferenceObjectPtr>>(constant::Name::Kids);
}

} // semantics
} // vanillapdf
