#include "precompiled.h"
#include "semantics/objects/page_tree.h"

#include "syntax/objects/array_object.h"

namespace gotchangpdf {
namespace semantics {

using namespace syntax;

PageTree::PageTree(DictionaryObjectPtr root) : HighLevelObject(root) {
	auto page_count = PageCount();
	m_pages.resize(page_count);
}

types::uinteger PageTree::PageCount(void) const {
	auto root = PageTreeNodePtr(_obj);
	auto result = root->KidCount()->SafeConvert<types::uinteger>();

	if (_obj->Contains(constant::Name::Count)) {
		auto count_obj = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Count);
		assert(count_obj->GetIntegerValue() == result);
	}

	return result;
}

PageObjectPtr PageTree::GetCachedPage(types::integer page_number) const {
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

	auto root = PageTreeNodePtr(_obj);
	types::integer pages_processed = 1;

	return PageInternal(root, page_number, pages_processed);
}

PageObjectPtr PageTree::PageInternal(PageTreeNodePtr node, types::integer page_number, types::integer& processed) const {
	auto kids = node->Kids();
	auto count = kids->Size();
	for (decltype(count) i = 0; i < count; ++i) {
		auto kid = kids->At(i);

		if (kid->GetNodeType() == PageNodeBase::NodeType::Tree) {
			auto tree_node = PageNodeUtils::ConvertTo<PageTreeNodePtr>(kid);
			auto kid_count = tree_node->KidCount()->SafeConvert<types::integer>();
			if (processed + kid_count <= page_number) {
				processed += kid_count;
				continue;
			}

			if (HasTreeChilds(tree_node)) {
				return PageInternal(tree_node, page_number, processed);
			}

			auto result = tree_node->Kids()->At(page_number - processed);
			auto page_object = PageNodeUtils::ConvertTo<PageObjectPtr>(result);
			m_pages[page_number - 1] = page_object;
			return page_object;
		}

		if (kid->GetNodeType() == PageNodeBase::NodeType::Object) {

			assert(processed <= page_number && "Current page shall never reach above the required page number");
			if (processed != page_number) {
				processed++;
				continue;
			}

			auto page_object = PageNodeUtils::ConvertTo<PageObjectPtr>(kid);
			m_pages[page_number - 1] = page_object;
			return page_object;
		}
	}

	throw GeneralException("Page number was not found: " + page_number);
}

bool PageTree::HasTreeChilds(PageTreeNodePtr node) const {
	auto kids = node->Kids();
	auto count = kids->Size();;
	for (decltype(count) i = 0; i < count; ++i) {
		auto kid = kids->At(i);
		if (kid->GetNodeType() == PageNodeBase::NodeType::Tree)
			return true;
	}

	return false;
}

void PageTree::Insert(PageObjectPtr object, types::integer index) {
	auto raw_obj = object->GetObject();
	auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
	kids->Insert(IndirectObjectReferencePtr(raw_obj), index);
	object->SetParent(PageTreeNodePtr(_obj));

	UpdateKidsCount(kids->Size());
	m_pages[index - 1] = object;
}

void PageTree::Append(PageObjectPtr object) {
	auto raw_obj = object->GetObject();
	auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
	kids->Append(IndirectObjectReferencePtr(raw_obj));
	object->SetParent(PageTreeNodePtr(_obj));

	UpdateKidsCount(kids->Size());
	m_pages.push_back(object);
}

void PageTree::Remove(types::integer index) {
	auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
	kids->Remove(index);

	UpdateKidsCount(kids->Size());
	m_pages.erase(m_pages.begin() + index - 1);
}

void PageTree::UpdateKidsCount(size_t new_size) {
	if (!_obj->Contains(constant::Name::Count)) {
		IntegerObjectPtr integer_size(new_size);
		_obj->Insert(constant::Name::Count, integer_size);
	}

	auto count = _obj->FindAs<IntegerObjectPtr>(constant::Name::Count);
	if (count->GetIntegerValue() != new_size) {
		count->SetValue(new_size);
	}
}

} // semantics
} // gotchangpdf
