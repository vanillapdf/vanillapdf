#include "precompiled.h"

#include "utils/attribute_interface.h"

namespace gotchangpdf {

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

} // gotchangpdf

namespace std {

size_t hash<gotchangpdf::IAttribute::Type>::operator()(gotchangpdf::IAttribute::Type attribute) const {
	std::hash<gotchangpdf::IAttribute::Type> hasher;
	return hasher(attribute);
}

} // std

