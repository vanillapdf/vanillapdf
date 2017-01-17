#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "syntax/objects/containable.h"

namespace gotchangpdf {
namespace syntax {

class BooleanObject : public ContainableObject {
public:
	BooleanObject() = default;
	explicit BooleanObject(bool value) : m_value(value) {}

	virtual Object::Type GetType(void) const noexcept override { return Object::Type::Boolean; }
	virtual std::string ToPdf(void) const override { return m_value ? "true" : "false"; }

	bool GetValue(void) const noexcept { return m_value; }
	void SetValue(bool value) noexcept { m_value = value; OnChanged(); }
	operator bool() const noexcept { return m_value; }

	virtual BooleanObject* Clone(void) const override { return new BooleanObject(*this); }
	virtual bool Equals(ObjectPtr other) const override;

private:
	bool m_value = false;
};

} // syntax
} // gotchangpdf

#endif /* _BOOLEAN_OBJECT_H */
