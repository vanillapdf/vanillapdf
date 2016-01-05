#include "precompiled.h"
#include "outline.h"

#include "exception.h"
#include "semantic_exceptions.h"
#include "semantic_utils.h"

namespace gotchangpdf
{
	namespace semantics
	{
		OutlineBasePtr OutlineItem::Parent(void) const
		{
			auto parent = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Parent);
			return OutlinePtr(parent);
		}
	}
}
