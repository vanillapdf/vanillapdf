#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

namespace gotchangpdf {
namespace syntax {

class NullObject : public ContainableObject {
public:
	virtual Object::Type GetType(void) const noexcept override { return Object::Type::Null; }
	virtual std::string ToPdf(void) const override { return "null"; }

	static NullObjectPtr GetInstance(void) {
		static NullObjectPtr instance = NullObjectPtr(pdf_new NullObject());
		return instance;
	}

	bool Equals(const NullObject&) const { return true; }
	virtual NullObject* Clone(void) const override { return new NullObject(*this); }

	bool operator==(const NullObject& other) const { return Equals(other); }
	bool operator!=(const NullObject& other) const { return !Equals(other); }
	bool operator<(const NullObject&) const { return false; }
};

} // syntax
} // gotchangpdf

#endif /* _NULL_OBJECT_H */
