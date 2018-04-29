#include "precompiled.h"

#include "utils/attribute_list.h"

namespace vanillapdf {

bool AttributeList::Contains(IAttribute::Type type) const {
	return (m_attributes.find(type) != m_attributes.end());
}

void AttributeList::Add(IAttributePtr attribute) {
	auto attribute_type = attribute->GetType();
	m_attributes[attribute_type] = attribute;
}

bool AttributeList::Remove(IAttribute::Type type) {
	auto found = m_attributes.find(type);
	if (found == m_attributes.end()) {
		return false;
	}

	m_attributes.erase(found);
	return true;
}

IAttributePtr AttributeList::Get(IAttribute::Type type) const {
	return m_attributes.at(type);
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

} // vanillapdf
