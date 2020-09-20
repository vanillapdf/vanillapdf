#include "precompiled.h"

#include "semantics/objects/outline.h"

#include "semantics/utils/semantic_exceptions.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

OutlineBase::OutlineBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
Outline::Outline(syntax::DictionaryObjectPtr root) : OutlineBase(root) {}
OutlineItemFlags::OutlineItemFlags(syntax::IntegerObjectPtr value) : HighLevelObject(value) {}
OutlineItem::OutlineItem(syntax::DictionaryObjectPtr root) : OutlineBase(root) {}

OutlineItemColor::OutlineItemColor(syntax::ArrayObjectPtr<syntax::RealObjectPtr> rgb) : HighLevelObject(rgb) {
	assert(rgb->GetSize() == 3);
	if (rgb->GetSize() != 3) {
		throw SemanticContextExceptionFactory::Construct<syntax::ArrayObject<syntax::RealObjectPtr>, OutlineItemColor>(rgb);
	}
}

syntax::StringObjectPtr OutlineItem::Title(void) const {
	return _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Title);
}

OutlineBasePtr OutlineItem::Parent(void) const {
	auto parent = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Parent);
	return make_deferred<Outline>(parent);
}

bool OutlineItem::Prev(OutputOutlineItemPtr& result) const {
	if (!_obj->Contains(constant::Name::Prev)) {
		return false;
	}

	auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Prev);
	result = make_deferred<OutlineItem>(item);
	return true;
}

bool OutlineItem::Next(OutputOutlineItemPtr& result) const {
	if (!_obj->Contains(constant::Name::Next)) {
		return false;
	}

	auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Next);
	result = make_deferred<OutlineItem>(item);
	return true;
}

bool OutlineItem::First(OutputOutlineItemPtr& result) const {
	if (!_obj->Contains(constant::Name::First)) {
		return false;
	}

	auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::First);
	result = make_deferred<OutlineItem>(item);
	return true;
}

bool OutlineItem::Last(OutputOutlineItemPtr& result) const {
	if (!_obj->Contains(constant::Name::Last)) {
		return false;
	}

	auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Last);
	result = make_deferred<OutlineItem>(item);
	return true;
}

bool OutlineItem::Count(syntax::IntegerObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Count)) {
		return false;
	}

	result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Count);
	return true;
}

bool OutlineItem::Color(OutputOutlineItemColorPtr& result) const {
	if (!_obj->Contains(constant::Name::C)) {
		return false;
	}

	auto color = _obj->FindAs<syntax::ArrayObjectPtr<syntax::RealObjectPtr>>(constant::Name::C);
	result = make_deferred<OutlineItemColor>(color);
	return true;
}

bool OutlineItem::Flags(OutputOutlineItemFlagsPtr& result) const {
	if (!_obj->Contains(constant::Name::F)) {
		return false;
	}

	auto flags = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::F);
	result = make_deferred<OutlineItemFlags>(flags);
	return true;
}

bool Outline::First(OutputOutlineItemPtr& result) const {
	if (!_obj->Contains(constant::Name::First)) {
		return false;
	}

	auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::First);
	result = make_deferred<OutlineItem>(item);
	return true;
}

bool Outline::Last(OutputOutlineItemPtr& result) const {
	if (!_obj->Contains(constant::Name::Last)) {
		return false;
	}

	auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Last);
	result = make_deferred<OutlineItem>(item);
	return true;
}

bool Outline::Count(syntax::IntegerObjectPtr& result) const {
	if (!_obj->Contains(constant::Name::Count)) {
		return false;
	}

	result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Count);
	return true;
}

syntax::RealObjectPtr OutlineItemColor::Red(void) const { return _obj->At(0); }
syntax::RealObjectPtr OutlineItemColor::Green(void) const { return _obj->At(1); }
syntax::RealObjectPtr OutlineItemColor::Blue(void) const { return _obj->At(2); }

bool OutlineItemFlags::Italic(void) const { return (_obj->GetIntegerValue() & 0x01) != 0; }
bool OutlineItemFlags::Bold(void) const { return (_obj->GetIntegerValue() & 0x02) != 0; }

} // semantics
} // vanillapdf
