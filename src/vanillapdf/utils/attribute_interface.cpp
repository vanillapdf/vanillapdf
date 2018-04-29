#include "precompiled.h"

#include "utils/attribute_interface.h"

namespace vanillapdf {

IAttributePtr::IAttributePtr() : Deferred<IAttribute>(make_deferred<EmptyAttribute>()) {

}

IAttribute::~IAttribute() {
}

bool IAttribute::Equals(const IAttribute& other) const {
	return (this == std::addressof(other));
}

bool operator==(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right) {
	return left->Equals(right);
}

bool operator!=(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right) {
	return !left->Equals(right);
}

EmptyAttribute* EmptyAttribute::Clone(void) const {
	return pdf_new EmptyAttribute();
}

} // vanillapdf

namespace std {

size_t hash<vanillapdf::IAttribute::Type>::operator()(vanillapdf::IAttribute::Type attribute) const {
	size_t converted = static_cast<size_t>(attribute);

	std::hash<size_t> hasher;
	return hasher(converted);
}

} // std

