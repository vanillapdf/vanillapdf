#ifndef OUTLINE_H
#define OUTLINE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/name_object.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/array_object.h"
#include "syntax/objects/integer_object.h"

namespace gotchangpdf {
namespace semantics {

class OutlineItemColor : public HighLevelObject<syntax::ArrayObjectPtr<syntax::RealObjectPtr>> {
public:
	explicit OutlineItemColor(syntax::ArrayObjectPtr<syntax::RealObjectPtr> rgb) : HighLevelObject(rgb) { assert(rgb->Size() == 3); }

	syntax::RealObjectPtr Red(void) const { return _obj->At(0); }
	syntax::RealObjectPtr Green(void) const { return _obj->At(1); }
	syntax::RealObjectPtr Blue(void) const { return _obj->At(2); }
};

class OutlineItemFlags : public HighLevelObject<syntax::IntegerObjectPtr> {
public:
	explicit OutlineItemFlags(syntax::IntegerObjectPtr value) : HighLevelObject(value) {}

	bool Italic(void) const noexcept { return (_obj->GetIntegerValue() & 0x01) != 0; }
	bool Bold(void) const noexcept { return (_obj->GetIntegerValue() & 0x02) != 0; }
};

class OutlineBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	enum class Type {
		Outline,
		Item
	};

	explicit OutlineBase(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}
	virtual Type OutlineType(void) const noexcept = 0;
};

class OutlineItem : public OutlineBase {
public:
	explicit OutlineItem(syntax::DictionaryObjectPtr root) : OutlineBase(root) {}
	virtual Type OutlineType(void) const noexcept override { return OutlineBase::Type::Item; }

	syntax::StringObjectPtr Title(void) const {
		return _obj->FindAs<syntax::StringObjectPtr>(constant::Name::Title);
	}

	OutlineBasePtr Parent(void) const;

	bool Prev(OutputOutlineItemPtr& result) const {
		if (!_obj->Contains(constant::Name::Prev))
			return false;

		auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Prev);
		result = OutlineItemPtr(item);
		return true;
	}

	bool Next(OutputOutlineItemPtr& result) const {
		if (!_obj->Contains(constant::Name::Next))
			return false;

		auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Next);
		result = OutlineItemPtr(item);
		return true;
	}

	bool First(OutputOutlineItemPtr& result) const {
		if (!_obj->Contains(constant::Name::First))
			return false;

		auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::First);
		result = OutlineItemPtr(item);
		return true;
	}

	bool Last(OutputOutlineItemPtr& result) const {
		if (!_obj->Contains(constant::Name::Last))
			return false;

		auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Last);
		result = OutlineItemPtr(item);
		return true;
	}

	bool Count(syntax::IntegerObjectPtr& result) const {
		if (!_obj->Contains(constant::Name::Count))
			return false;

		result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Count);
		return true;
	}

	//TODO destinations
	//bool Destination(syntax::ObjectPtr& result) const
	//{
	//	if (!_obj->Contains(constant::Name::Dest))
	//		return false;

	//	result = _obj->Find(constant::Name::Dest);
	//	return true;
	//}

	//TODO action dictionary
	//bool Action(syntax::ObjectPtr& result) const
	//{
	//}

	//TODO structure hierarchy
	//bool StructureElement(syntax::ObjectPtr& result) const
	//{
	//}

	bool Color(OutputOutlineItemColorPtr& result) const {
		if (!_obj->Contains(constant::Name::C))
			return false;

		auto color = _obj->FindAs<syntax::ArrayObjectPtr<syntax::RealObjectPtr>>(constant::Name::C);
		result = OutlineItemColorPtr(color);
		return true;
	}

	bool Flags(OutputOutlineItemFlagsPtr& result) const {
		if (!_obj->Contains(constant::Name::F))
			return false;

		auto flags = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::F);
		result = OutlineItemFlagsPtr(flags);
		return true;
	}
};

class Outline : public OutlineBase {
public:
	explicit Outline(syntax::DictionaryObjectPtr root) : OutlineBase(root) {}
	virtual Type OutlineType(void) const noexcept override { return OutlineBase::Type::Outline; }

	bool First(OutputOutlineItemPtr& result) const {
		if (!_obj->Contains(constant::Name::First))
			return false;

		auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::First);
		result = OutlineItemPtr(item);
		return true;
	}

	bool Last(OutputOutlineItemPtr& result) const {
		if (!_obj->Contains(constant::Name::Last))
			return false;

		auto item = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Last);
		result = OutlineItemPtr(item);
		return true;
	}

	bool Count(syntax::IntegerObjectPtr& result) const {
		if (!_obj->Contains(constant::Name::Count))
			return false;

		result = _obj->FindAs<syntax::IntegerObjectPtr>(constant::Name::Count);
		return true;
	}
};

} // semantics
} // gotchangpdf

#endif /* OUTLINE_H */
