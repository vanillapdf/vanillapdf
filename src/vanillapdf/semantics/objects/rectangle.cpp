#include "precompiled.h"
#include "semantics/objects/rectangle.h"

namespace vanillapdf {
namespace semantics {

Rectangle::Rectangle(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> list) : HighLevelObject(list) {
	assert(list->GetSize() == 4 && "Only fully specified rectangles are yet supported");

	m_llx = list->GetValue(0);
	m_lly = list->GetValue(1);
	m_urx = list->GetValue(2);
	m_ury = list->GetValue(3);
}

} // semantics
} // vanillapdf
