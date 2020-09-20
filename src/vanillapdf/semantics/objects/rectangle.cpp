#include "precompiled.h"
#include "semantics/objects/rectangle.h"

namespace vanillapdf {
namespace semantics {

Rectangle::Rectangle(syntax::ArrayObjectPtr<syntax::IntegerObjectPtr> list) : HighLevelObject(list) {
	assert(list->GetSize() == 4 && "Only fully specified rectangles are yet supported");

	m_llx = list->At(0);
	m_lly = list->At(1);
	m_urx = list->At(2);
	m_ury = list->At(3);
}

} // semantics
} // vanillapdf
