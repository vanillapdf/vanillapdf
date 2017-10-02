#ifndef OUTLINE_H
#define OUTLINE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

#include "syntax/objects/name_object.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/array_object.h"
#include "syntax/objects/integer_object.h"

namespace vanillapdf {
namespace semantics {

class OutlineItemColor : public HighLevelObject<syntax::ArrayObjectPtr<syntax::RealObjectPtr>> {
public:
	explicit OutlineItemColor(syntax::ArrayObjectPtr<syntax::RealObjectPtr> rgb);

	syntax::RealObjectPtr Red(void) const;
	syntax::RealObjectPtr Green(void) const;
	syntax::RealObjectPtr Blue(void) const;
};

class OutlineItemFlags : public HighLevelObject<syntax::IntegerObjectPtr> {
public:
	explicit OutlineItemFlags(syntax::IntegerObjectPtr value);

	bool Italic(void) const;
	bool Bold(void) const;
};

class OutlineBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	enum class Type {
		Outline,
		Item
	};

	explicit OutlineBase(syntax::DictionaryObjectPtr root);
	virtual Type OutlineType(void) const noexcept = 0;
};

class OutlineItem : public OutlineBase {
public:
	explicit OutlineItem(syntax::DictionaryObjectPtr root);
	virtual Type OutlineType(void) const noexcept override;

	syntax::StringObjectPtr Title(void) const;
	OutlineBasePtr Parent(void) const;
	bool Prev(OutputOutlineItemPtr& result) const;
	bool Next(OutputOutlineItemPtr& result) const;
	bool First(OutputOutlineItemPtr& result) const;
	bool Last(OutputOutlineItemPtr& result) const;
	bool Count(syntax::IntegerObjectPtr& result) const;

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

	bool Color(OutputOutlineItemColorPtr& result) const;
	bool Flags(OutputOutlineItemFlagsPtr& result) const;
};

class Outline : public OutlineBase {
public:
	explicit Outline(syntax::DictionaryObjectPtr root);
	virtual Type OutlineType(void) const noexcept override;

	bool First(OutputOutlineItemPtr& result) const;
	bool Last(OutputOutlineItemPtr& result) const;
	bool Count(syntax::IntegerObjectPtr& result) const;
};

inline OutlineBase::Type OutlineItem::OutlineType(void) const noexcept {
	return OutlineBase::Type::Item;
}

inline OutlineBase::Type Outline::OutlineType(void) const noexcept {
	return OutlineBase::Type::Outline;
}

} // semantics
} // vanillapdf

#endif /* OUTLINE_H */
