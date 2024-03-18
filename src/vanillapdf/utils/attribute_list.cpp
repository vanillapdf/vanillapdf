#include "precompiled.h"

#include "utils/attribute_list.h"

namespace vanillapdf {

std::shared_ptr<AttributeList::map_type> AttributeList::GetAttributes() const {
	if (!m_attributes) {
		m_attributes = std::make_shared<map_type>();
	}

	return m_attributes;
}

bool AttributeList::Contains(BaseAttribute::Type type) const {
	// Optimization:
	// The unordered map allocates 16 empty entries,
	// but most of the object do not contain any attributes at all
	if (!m_attributes) {
		return false;
	}

	return (GetAttributes()->find(type) != GetAttributes()->end());
}

void AttributeList::Add(BaseAttributePtr attribute) {
	auto attribute_type = attribute->GetType();
	(*GetAttributes())[attribute_type] = attribute;
}

bool AttributeList::Remove(BaseAttribute::Type type) {
	auto found = GetAttributes()->find(type);
	if (found == GetAttributes()->end()) {
		return false;
	}

	GetAttributes()->erase(found);
	return true;
}

BaseAttributePtr AttributeList::Get(BaseAttribute::Type type) const {
	return GetAttributes()->at(type);
}

void AttributeList::Clear() {
	GetAttributes()->clear();
}

AttributeList::iterator AttributeList::begin() {
	return GetAttributes()->begin();
}

AttributeList::const_iterator AttributeList::begin() const {
	return GetAttributes()->begin();
}

AttributeList::iterator AttributeList::end() {
	return GetAttributes()->end();
}

AttributeList::const_iterator AttributeList::end() const {
	return GetAttributes()->end();
}

} // vanillapdf
