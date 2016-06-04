#include "precompiled.h"
#include "document_info.h"
#include "date.h"

namespace gotchangpdf
{
	namespace semantics
	{
		DocumentInfo::DocumentInfo(syntax::DictionaryObjectPtr root) : HighLevelObject(root)
		{

		}

		bool DocumentInfo::Title(syntax::OutputStringObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Title))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Title);
			return true;
		}

		bool DocumentInfo::Author(syntax::OutputStringObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Author))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Author);
			return true;
		}

		bool DocumentInfo::Subject(syntax::OutputStringObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Subject))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Subject);
			return true;
		}

		bool DocumentInfo::Keywords(syntax::OutputStringObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Keywords))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Keywords);
			return true;
		}

		bool DocumentInfo::Creator(syntax::OutputStringObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Creator))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Creator);
			return true;
		}

		bool DocumentInfo::Producer(syntax::OutputStringObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Producer))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Producer);
			return true;
		}

		bool DocumentInfo::CreationDate(OutputDatePtr& result) const
		{
			if (!_obj->Contains(constant::Name::CreationDate))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::CreationDate);
			return true;
		}

		bool DocumentInfo::ModificationDate(OutputDatePtr& result) const
		{
			if (!_obj->Contains(constant::Name::ModDate))
				return false;

			result = _obj->FindAs<syntax::StringObjectPtr>(constant::Name::ModDate);
			return true;
		}

		bool DocumentInfo::Trapped(DocumentTrapped& result) const
		{
			if (!_obj->Contains(constant::Name::Trapped))
				return false;

			auto trapped = _obj->Find(constant::Name::Trapped);
			BufferPtr value;
			if (syntax::ObjectUtils::IsType<syntax::NameObjectPtr>(trapped)) {
				auto name = syntax::ObjectUtils::ConvertTo<syntax::NameObjectPtr>(trapped);
				value = name->GetValue();
			}

			if (syntax::ObjectUtils::IsType<syntax::StringObjectPtr>(trapped)) {
				auto string = syntax::ObjectUtils::ConvertTo<syntax::StringObjectPtr>(trapped);
				value = string->GetValue();
			}

			if (*value == *constant::Name::True) {
				result = DocumentTrapped::True;
				return true;
			}

			if (*value == *constant::Name::False) {
				result = DocumentTrapped::False;
				return true;
			}

			if (*value == *constant::Name::Unknown) {
				result = DocumentTrapped::Unknown;
				return true;
			}

			assert(false && "Document trapped is neither of name or string");
			return false;
		}
	}
}