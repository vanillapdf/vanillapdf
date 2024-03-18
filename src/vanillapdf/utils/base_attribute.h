#ifndef _BASE_ATTRIBUTE_H
#define _BASE_ATTRIBUTE_H

#include "utils/unknown_interface.h"

namespace vanillapdf {

class BaseAttribute : public IUnknown {
public:
	enum class Type {
		Undefined = 0,
		Empty,
		SerializationOverride,
		TrackingIdentifier
	};

	virtual bool Equals(const BaseAttribute& other) const;
	virtual Type GetType(void) const noexcept = 0;
	virtual BaseAttribute* Clone(void) const = 0;
	virtual ~BaseAttribute() = 0;
};

class EmptyAttribute : public BaseAttribute {
public:
	virtual Type GetType(void) const noexcept override {
		return Type::Empty;
	}

	virtual EmptyAttribute* Clone(void) const override;
};

class BaseAttributePtr : public Deferred<BaseAttribute> {
public:
	using Deferred<BaseAttribute>::Deferred;

	BaseAttributePtr();
};

bool operator==(const Deferred<BaseAttribute>& left, const Deferred<BaseAttribute>& right);
bool operator!=(const Deferred<BaseAttribute>& left, const Deferred<BaseAttribute>& right);

} // vanillapdf

namespace std {

template <> struct hash<vanillapdf::BaseAttribute::Type> {
	size_t operator()(vanillapdf::BaseAttribute::Type attribute) const;
};

} // std

#endif /* _BASE_ATTRIBUTE_H */
