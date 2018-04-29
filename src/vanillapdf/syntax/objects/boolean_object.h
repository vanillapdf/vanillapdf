#ifndef _BOOLEAN_OBJECT_H
#define _BOOLEAN_OBJECT_H

#include "syntax/objects/containable.h"

namespace vanillapdf {
namespace syntax {

class BooleanObject : public ContainableObject {
public:
	BooleanObject() = default;
	explicit BooleanObject(bool value);

	virtual Object::Type GetType(void) const noexcept override { return Object::Type::Boolean; }
	virtual void ToPdfStream(IOutputStreamPtr output) const override;

	bool GetValue(void) const noexcept;
	void SetValue(bool value);
	operator bool() const noexcept;

	virtual size_t Hash() const override;
	virtual BooleanObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;

private:
	bool m_value = false;
};

} // syntax
} // vanillapdf

#endif /* _BOOLEAN_OBJECT_H */
