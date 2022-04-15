#ifndef _ATTRIBUTE_INTERFACE_H
#define _ATTRIBUTE_INTERFACE_H

#include "utils/unknown_interface.h"

namespace vanillapdf {

class IAttribute : public virtual IUnknown {
public:
	enum class Type {
		Undefined = 0,
		Empty,
		SerializationOverride,
		TrackingIdentifier
	};

	virtual bool Equals(const IAttribute& other) const;
	virtual Type GetType(void) const noexcept = 0;
	virtual IAttribute* Clone(void) const = 0;
	virtual ~IAttribute() = 0;
};

class EmptyAttribute : public virtual IAttribute {
public:
	virtual Type GetType(void) const noexcept override;

	virtual EmptyAttribute* Clone(void) const override;
};

class IAttributePtr : public Deferred<IAttribute> {
public:
	using Deferred<IAttribute>::Deferred;

	IAttributePtr();
};

bool operator==(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right);
bool operator!=(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right);

inline IAttribute::Type EmptyAttribute::GetType() const noexcept {
	return Type::Empty;
}

} // vanillapdf

namespace std {

template <> struct hash<vanillapdf::IAttribute::Type> {
	size_t operator()(vanillapdf::IAttribute::Type attribute) const;
};

} // std

#endif /* _ATTRIBUTE_INTERFACE_H */
