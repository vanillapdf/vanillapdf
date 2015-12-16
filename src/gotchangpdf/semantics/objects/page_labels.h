#ifndef _PAGE_LABEL_H
#define _PAGE_LABEL_H

#include "semantics_fwd.h"
#include "high_level_object.h"
#include "semantic_exceptions.h"

#include "object_utils.h"
#include "dictionary_object.h"
#include "integer_object.h"
#include "name_object.h"
#include "string_object.h"
#include "tree.h"

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
					throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, Catalog>(obj);
			}

			syntax::StringObjectPtr P(void) const
			{
				if (!_obj->Contains(constant::Name::P))
					throw OptionalEntryMissingException(_obj, constant::Name::P);

				return _obj->FindAs<syntax::StringObjectPtr>(constant::Name::P);
			}
			syntax::IntegerObjectPtr St(void) const
			{
				if (!_obj->Contains(constant::Name::St))
					throw OptionalEntryMissingException(_obj, constant::Name::St);

				return _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::St);
			}

			NumberingStyle S(void) const
			{
				if (!_obj->Contains(constant::Name::S))
					throw OptionalEntryMissingException(_obj, constant::Name::S);

				auto value = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::S);
				auto buf = value->Value();
				if (buf->size() != 1)
					throw GeneralException("Unknown numbering style" + buf->ToString());

				if (buf[0] != static_cast<char>(NumberingStyle::Decimal)
					&& buf[0] != static_cast<char>(NumberingStyle::UpperRoman)
					&& buf[0] != static_cast<char>(NumberingStyle::LowerRoman)
					&& buf[0] != static_cast<char>(NumberingStyle::UpperLetters)
					&& buf[0] != static_cast<char>(NumberingStyle::LowerLetters))
					throw GeneralException("Unknown numbering style" + buf->ToString());

				return static_cast<NumberingStyle>(buf[0]);
			}
		};

		class PageLabels : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			explicit PageLabels(const syntax::DictionaryObjectPtr& obj) : HighLevelObject(obj)
			{
				_tree = NumberTreePtr<PageLabelPtr>(obj, [](const syntax::ContainableObjectPtr& item) {
					auto dict = syntax::ObjectUtils::ConvertTo<syntax::DictionaryObjectPtr>(item);
					return PageLabelPtr(dict);
				});
			}

			bool Contains(types::uinteger page_number) const { return _tree->Contains(page_number); }
			PageLabelPtr Find(types::uinteger page_number) const { return _tree->Find(page_number); }

		private:
			NumberTreePtr<PageLabelPtr> _tree;
		};
	}
}

#endif /* _PAGE_LABEL_H */
