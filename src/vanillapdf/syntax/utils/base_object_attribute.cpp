#include "precompiled.h"

#include "syntax/utils/base_object_attribute.h"
#include "syntax/utils/empty_object_attribute.h"

namespace vanillapdf {
namespace syntax {

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

} // syntax
} // vanillapdf

namespace std {

size_t hash<vanillapdf::syntax::BaseAttribute::Type>::operator()(vanillapdf::syntax::BaseAttribute::Type attribute) const {
	size_t converted = static_cast<size_t>(attribute);

	std::hash<size_t> hasher;
	return hasher(converted);
}

} // std

