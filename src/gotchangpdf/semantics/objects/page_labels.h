#ifndef _PAGE_LABEL_H
#define _PAGE_LABEL_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/utils/semantic_exceptions.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/tree.h"

#include "syntax/utils/object_utils.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/integer_object.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/string_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class PageLabel : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			enum class NumberingStyle : char
			{
				Decimal = 'D',
				UpperRoman = 'R',
				LowerRoman = 'r',
				UpperLetters = 'A',
				LowerLetters = 'a'
			};

		public:
			explicit PageLabel(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj)
			{
				if (_obj->Contains(constant::Name::Type) && _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Type) != constant::Name::PageLabel)
					throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageLabel>(obj);
			}

			bool Prefix(syntax::StringObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::P))
					return false;

				result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::P);
				return true;
			}

			bool Start(syntax::IntegerObjectPtr& result) const
			{
				if (!_obj->Contains(constant::Name::St))
					return false;

				result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::St);
				return true;
			}

			bool Style(NumberingStyle& result) const
			{
				if (!_obj->Contains(constant::Name::S))
					return false;

				auto value = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::S);
				auto buf = value->GetValue();
				if (buf->size() != 1)
					throw GeneralException("Unknown numbering style" + buf->ToString());

				if (buf[0] != static_cast<char>(NumberingStyle::Decimal)
					&& buf[0] != static_cast<char>(NumberingStyle::UpperRoman)
					&& buf[0] != static_cast<char>(NumberingStyle::LowerRoman)
					&& buf[0] != static_cast<char>(NumberingStyle::UpperLetters)
					&& buf[0] != static_cast<char>(NumberingStyle::LowerLetters))
					throw GeneralException("Unknown numbering style" + buf->ToString());

				result = static_cast<NumberingStyle>(buf[0]);
				return true;
			}
		};

		class PageLabels : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit PageLabels(const syntax::DictionaryObjectPtr& obj)
				: HighLevelObject(obj),
				_tree
				(
					NumberTreePtr<PageLabelPtr>(obj, [](const syntax::ContainableObjectPtr& item) {
						auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(item);
						return PageLabelPtr(dict);
					})
				) {}

			bool Contains(types::uinteger page_number) const { return _tree->Contains(page_number); }
			PageLabelPtr Find(types::uinteger page_number) const { return _tree->Find(page_number); }

		private:
			NumberTreePtr<PageLabelPtr> _tree;
		};
	}
}

#endif /* _PAGE_LABEL_H */
