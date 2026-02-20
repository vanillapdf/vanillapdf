#include "precompiled.h"
#include "semantics/objects/page_tree.h"

#include "syntax/files/file.h"
#include "syntax/objects/array_object.h"
#include "syntax/utils/name_constants.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include <fmt/core.h>

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

    throw syntax::ParseException("Unknown page object type");
}

PageObjectPtr PageTree::GetCachedPage(types::size_type page_number) const {
    if (page_number < 1) {
        throw InvalidParameterException(fmt::format("Invalid page number: {}", page_number));
    }

    if (m_page_cache.empty()) {
        BuildPageCache();
    }

    if (page_number > m_page_cache.size()) {
        throw ObjectMissingException("Page number was not found: " + std::to_string(page_number));
    }

    spdlog::debug("Searching for page {}", page_number);

    return m_page_cache[page_number - 1];
}

void PageTree::BuildPageCache() const {
    m_page_cache.clear();
    m_page_cache.reserve(PageCount());
    auto root = make_deferred<PageTreeNode>(_obj);
    BuildPageCacheInternal(root);
}

void PageTree::BuildPageCacheInternal(PageTreeNodePtr node) const {
    auto kids = node->Kids();
    auto count = kids->GetSize();
    for (decltype(count) i = 0; i < count; ++i) {
        auto kid = kids->GetValue(i);

        if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
            auto tree_node = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageTreeNodePtr>(kid);
            BuildPageCacheInternal(tree_node);
        } else {
            auto page_object = ConvertUtils<PageNodeBasePtr>::ConvertTo<PageObjectPtr>(kid);
            m_page_cache.push_back(page_object);
        }
    }
}

void PageTree::InvalidatePageCache() noexcept {
    m_page_cache.clear();
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

    throw ObjectMissingException("Page number was not found: " + std::to_string(page_number));
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
