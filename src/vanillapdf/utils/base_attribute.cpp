#include "precompiled.h"

#include "utils/base_attribute.h"

namespace vanillapdf {

BaseAttributePtr::BaseAttributePtr() : Deferred<BaseAttribute>(make_deferred<EmptyAttribute>()) {

}

BaseAttribute::~BaseAttribute() {
}

bool BaseAttribute::Equals(const BaseAttribute& other) const {
	return (this == std::addressof(other));
}

bool operator==(const Deferred<BaseAttribute>& left, const Deferred<BaseAttribute>& right) {
	return left->Equals(right);
}

bool operator!=(const Deferred<BaseAttribute>& left, const Deferred<BaseAttribute>& right) {
	return !left->Equals(right);
}

EmptyAttribute* EmptyAttribute::Clone(void) const {
	return pdf_new EmptyAttribute();
}

} // vanillapdf

namespace std {

size_t hash<vanillapdf::BaseAttribute::Type>::operator()(vanillapdf::BaseAttribute::Type attribute) const {
	size_t converted = static_cast<size_t>(attribute);

	std::hash<size_t> hasher;
	return hasher(converted);
}

} // std

