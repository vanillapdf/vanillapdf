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

bool AttributeList::Contains(IAttribute::Type type) const {
	return (m_attributes.find(type) != m_attributes.end());
}

void AttributeList::Add(IAttributePtr attribute) {
	auto attribute_type = attribute->GetType();
	m_attributes[attribute_type] = attribute;
}

bool AttributeList::Remove(IAttributePtr attribute) {
	auto attribute_type = attribute->GetType();
	auto found = m_attributes.find(attribute_type);
	if (found == m_attributes.end()) {
		return false;
	}

	m_attributes.erase(found);
	return true;
}

IAttributePtr AttributeList::Get(IAttribute::Type type) {
	return m_attributes[type];
}

void AttributeList::Clear() {
	m_attributes.clear();
}

AttributeList::iterator AttributeList::begin() noexcept {
	return m_attributes.begin();
}

AttributeList::const_iterator AttributeList::begin() const noexcept {
	return m_attributes.begin();
}

AttributeList::iterator AttributeList::end() noexcept {
	return m_attributes.end();
}

AttributeList::const_iterator AttributeList::end() const noexcept {
	return m_attributes.end();
}

bool operator==(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right) {
	return left->Equals(right);
}

bool operator!=(const Deferred<IAttribute>& left, const Deferred<IAttribute>& right) {
	return !left->Equals(right);
}

} // gotchangpdf

namespace std {

size_t hash<gotchangpdf::IAttribute>::operator()(const gotchangpdf::IAttribute& attribute) const {
	std::hash<gotchangpdf::IAttribute::Type> hasher;
	return hasher(attribute.GetType());
}

} // std

