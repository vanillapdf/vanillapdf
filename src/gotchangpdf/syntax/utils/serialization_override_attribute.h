#ifndef _SERIALIZATION_OVERRIDE_ATTRIBUTE_H
#define _SERIALIZATION_OVERRIDE_ATTRIBUTE_H

#include "utils/attribute_interface.h"

#include <string>

namespace gotchangpdf {
namespace syntax {

class SerializationOverrideAttribute : public IAttribute {
public:
	SerializationOverrideAttribute() = default;
	SerializationOverrideAttribute(std::string value);

	std::string GetValue(void) const;

	virtual Type GetType(void) const noexcept override;

private:
	std::string m_value;
};

inline SerializationOverrideAttribute::SerializationOverrideAttribute(std::string value) {
	m_value = value;
}

inline std::string SerializationOverrideAttribute::GetValue(void) const {
	return m_value;
}

inline IAttribute::Type SerializationOverrideAttribute::GetType(void) const noexcept {
	return IAttribute::Type::SerializationOverride;
}

} // syntax
} // gotchangpdf

#endif /* _SERIALIZATION_OVERRIDE_ATTRIBUTE_H */
