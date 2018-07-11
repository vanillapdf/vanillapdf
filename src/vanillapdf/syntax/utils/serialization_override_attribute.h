#ifndef _SERIALIZATION_OVERRIDE_ATTRIBUTE_H
#define _SERIALIZATION_OVERRIDE_ATTRIBUTE_H

#include "utils/attribute_interface.h"

#include <string>

namespace vanillapdf {
namespace syntax {

class SerializationOverrideAttribute : public IAttribute {
public:
	SerializationOverrideAttribute() = default;
	explicit SerializationOverrideAttribute(const std::string& value);

	std::string GetValue(void) const;

	virtual Type GetType(void) const noexcept override;
	virtual SerializationOverrideAttribute* Clone(void) const;

private:
	std::string m_value;
};

inline SerializationOverrideAttribute::SerializationOverrideAttribute(const std::string& value) : m_value(value) {
}

inline std::string SerializationOverrideAttribute::GetValue(void) const {
	return m_value;
}

inline IAttribute::Type SerializationOverrideAttribute::GetType(void) const noexcept {
	return IAttribute::Type::SerializationOverride;
}

inline SerializationOverrideAttribute* SerializationOverrideAttribute::Clone(void) const {
	return pdf_new SerializationOverrideAttribute(m_value);
}

} // syntax
} // vanillapdf

#endif /* _SERIALIZATION_OVERRIDE_ATTRIBUTE_H */
