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

		bool DocumentInfo::Trapped(syntax::OutputNameObjectPtr& result) const
		{
			if (!_obj->Contains(constant::Name::Trapped))
				return false;

			result = _obj->FindAs<syntax::NameObjectPtr>(constant::Name::Trapped);
			return true;
		}
	}
}