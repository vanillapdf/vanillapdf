#ifndef _SERIALIZATION_OVERRIDE_ATTRIBUTE_H
#define _SERIALIZATION_OVERRIDE_ATTRIBUTE_H

#include "utils/attribute_interface.h"

#include <string>

namespace vanillapdf {
namespace syntax {

class SerializationOverrideAttribute : public IAttribute {
public:
	SerializationOverrideAttribute() = default;

	explicit SerializationOverrideAttribute(const std::string& value) : m_value(value) {

	}

	std::string GetValue(void) const {
		return m_value;
	}

	virtual Type GetType(void) const noexcept override {
		return IAttribute::Type::SerializationOverride;
	}

	virtual SerializationOverrideAttribute* Clone(void) const override {
		return pdf_new SerializationOverrideAttribute(m_value);
	}

private:
	std::string m_value;
};

} // syntax
} // vanillapdf

#endif /* _SERIALIZATION_OVERRIDE_ATTRIBUTE_H */
