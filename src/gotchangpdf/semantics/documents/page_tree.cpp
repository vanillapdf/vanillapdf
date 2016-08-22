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

		PageTree::PageTree(DictionaryObjectPtr root) : HighLevelObject(root), _root(root) {}

		PageObjectPtr PageTree::PageInternal(types::integer number) const
		{
			auto node = _root;
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
						return PageNodeUtils::ConvertTo<PageObjectPtr>(result);
					}
					else
					{
						assert(current + under >= current);
						current += under;
					}

					break;
				}
				case PageNodeBase::NodeType::Object:
					if (current == number)
						return PageNodeUtils::ConvertTo<PageObjectPtr>(kid);
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

			UpdateKidsCount(kids->Size());
		}

		void PageTree::Append(PageObjectPtr object)
		{
			auto raw_obj = object->GetObject();
			auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
			kids->Append(IndirectObjectReferencePtr(raw_obj));

			UpdateKidsCount(kids->Size());
		}

		void PageTree::Remove(types::integer index)
		{
			auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(constant::Name::Kids);
			kids->Remove(index);

			UpdateKidsCount(kids->Size());
		}

		void PageTree::UpdateKidsCount(size_t new_size)
		{
			if (!_obj->Contains(constant::Name::Count)) {
				IntegerObjectPtr integer_size(new_size);
				_obj->Insert(constant::Name::Count, integer_size);
			}

			auto count = _obj->FindAs<IntegerObjectPtr>(constant::Name::Count);
			if (count->GetValue() != new_size) {
				count->SetValue(new_size);
			}
		}
	}
}
