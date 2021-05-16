#include "precompiled.h"

#include "utils/attribute_list.h"

namespace vanillapdf {

std::shared_ptr<AttributeList::map_type> AttributeList::GetAttributes() const {
	if (!m_attributes) {
		m_attributes = std::make_shared<map_type>();
	}

	return m_attributes;
}

bool AttributeList::Contains(IAttribute::Type type) const {
	// Optimization:
	// The unordered map allocates 16 empty entries,
	// but most of the object do not contain any attributes at all
	if (!m_attributes) {
		return false;
	}

	return (GetAttributes()->find(type) != GetAttributes()->end());
}

void AttributeList::Add(IAttributePtr attribute) {
	auto attribute_type = attribute->GetType();
	(*GetAttributes())[attribute_type] = attribute;
}

bool AttributeList::Remove(IAttribute::Type type) {
	auto found = GetAttributes()->find(type);
	if (found == GetAttributes()->end()) {
		return false;
	}

	GetAttributes()->erase(found);
	return true;
}

IAttributePtr AttributeList::Get(IAttribute::Type type) const {
	return GetAttributes()->at(type);
}

void AttributeList::Clear() {
	GetAttributes()->clear();
}

AttributeList::iterator AttributeList::begin() noexcept {
	return GetAttributes()->begin();
}

AttributeList::const_iterator AttributeList::begin() const noexcept {
	return GetAttributes()->begin();
}

AttributeList::iterator AttributeList::end() noexcept {
	return GetAttributes()->end();
}

AttributeList::const_iterator AttributeList::end() const noexcept {
	return GetAttributes()->end();
}

} // vanillapdf
