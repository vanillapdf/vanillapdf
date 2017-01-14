#include "precompiled.h"
#include "semantics/objects/outline.h"

namespace gotchangpdf {
namespace semantics {

OutlineBasePtr OutlineItem::Parent(void) const {
	auto parent = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Parent);
	return OutlinePtr(parent);
}

} // semantics
} // gotchangpdf
