#ifndef _BASE_OBJECT_ATTRIBUTE_H
#define _BASE_OBJECT_ATTRIBUTE_H

#include "utils/unknown_interface.h"

namespace vanillapdf {
namespace syntax {

class BaseAttribute : public IUnknown {
public:
	enum class Type {
		Undefined = 0,
		Empty,
		SerializationOverride,
		TrackingIdentifier,
		ImageMetadata
	};

	virtual bool Equals(const BaseAttribute& other) const;
	virtual Type GetType(void) const noexcept = 0;
	virtual BaseAttribute* Clone(void) const = 0;
	virtual ~BaseAttribute() = 0;
};

class BaseAttributePtr : public Deferred<BaseAttribute> {
public:
	using Deferred<BaseAttribute>::Deferred;

	BaseAttributePtr();
};

bool operator==(const Deferred<BaseAttribute>& left, const Deferred<BaseAttribute>& right);
bool operator!=(const Deferred<BaseAttribute>& left, const Deferred<BaseAttribute>& right);

} // syntax
} // vanillapdf

namespace std {

template <> struct hash<vanillapdf::syntax::BaseAttribute::Type> {
	size_t operator()(vanillapdf::syntax::BaseAttribute::Type attribute) const;
};

} // std

#endif /* _BASE_OBJECT_ATTRIBUTE_H */
