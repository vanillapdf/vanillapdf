#include "precompiled.h"
#include "page_tree.h"

#include "array_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;
		using namespace syntax;

		//PageTree::PageTree() {}

		PageTree::PageTree(DictionaryObjectPtr root) : HighLevelObject(root)
		{
			auto page_count = PageCount();
			m_pages.resize(page_count);
		}

		types::uinteger PageTree::PageCount(void) const
		{
			auto root = PageTreeNodePtr(_obj);
			auto result = root->KidCount()->SafeConvert<types::uinteger>();

			if (_obj->Contains(constant::Name::Count)) {
				auto count_obj = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Count);
				assert(count_obj->GetIntegerValue() == result);
			}

			return result;
		}

		PageObjectPtr PageTree::PageInternal(types::integer number) const
		{
			auto found_page = m_pages.at(number - 1);
			if (!found_page.empty()) {
				return found_page;
			}

			auto root = PageTreeNodePtr(_obj);
			auto node = root;
			types::integer current = 1;

		dive:
			auto kids = node->Kids();
			auto count = kids->Size();
			for (decltype(count) i = 0; i < count; ++i) {
				auto kid = kids->At(i);
				switch (kid->GetNodeType()) {
				case PageNodeBase::NodeType::Tree:
				{
					auto tree_node = PageNodeUtils::ConvertTo<PageTreeNodePtr>(kid);
					auto under = tree_node->KidCount()->SafeConvert<types::integer>();
					if (current + under > number)
					{
						if (HasTreeChilds(tree_node))
						{
							node = tree_node;
							goto dive;
						}

						auto result = tree_node->Kids()->At(number - current);
						auto page_object = PageNodeUtils::ConvertTo<PageObjectPtr>(result);
						m_pages[number - 1] = page_object;
						return page_object;
					}
					else
					{
						assert(current + under >= current);
						current += under;
					}

					break;
				}
				case PageNodeBase::NodeType::Object:
					if (current == number) {
						auto page_object = PageNodeUtils::ConvertTo<PageObjectPtr>(kid);
						m_pages[number - 1] = page_object;
						return page_object;
					}
					else
						current++;
					break;
				default:
					assert(!"Kids array contains item with invalid type");
					break;
				}
			}

			throw GeneralException("Page number was not found: " + number);
		}

		bool PageTree::HasTreeChilds(PageTreeNodePtr node) const
		{
			auto kids = node->Kids();
			auto count = kids->Size();;
			for (decltype(count) i = 0; i < count; ++i) {
				auto kid = kids->At(i);
				if (kid->GetNodeType() == PageNodeBase::NodeType::Tree)
					return true;
			}

			return false;
		}

		void PageTree::Insert(PageObjectPtr object, types::integer index)
		{
			auto raw_obj = object->GetObject();
			auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
			kids->Insert(IndirectObjectReferencePtr(raw_obj), index);
			object->SetParent(PageTreeNodePtr(_obj));

			UpdateKidsCount(kids->Size());
			m_pages[index - 1] = object;
		}

		void PageTree::Append(PageObjectPtr object)
		{
			auto raw_obj = object->GetObject();
			auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
			kids->Append(IndirectObjectReferencePtr(raw_obj));
			object->SetParent(PageTreeNodePtr(_obj));

			UpdateKidsCount(kids->Size());
			m_pages.push_back(object);
		}

		void PageTree::Remove(types::integer index)
		{
			auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
			kids->Remove(index);

			UpdateKidsCount(kids->Size());
			m_pages.erase(m_pages.begin() + index - 1);
		}

		void PageTree::UpdateKidsCount(size_t new_size)
		{
			if (!_obj->Contains(constant::Name::Count)) {
				IntegerObjectPtr integer_size(new_size);
				_obj->Insert(constant::Name::Count, integer_size);
			}

			auto count = _obj->FindAs<IntegerObjectPtr>(constant::Name::Count);
			if (count->GetIntegerValue() != new_size) {
				count->SetValue(new_size);
			}
		}
	}
}
