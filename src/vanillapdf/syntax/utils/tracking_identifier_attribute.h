#ifndef _TRACKING_IDENTIFIER_ATTRIBUTE_H
#define _TRACKING_IDENTIFIER_ATTRIBUTE_H

#include "utils/attribute_interface.h"

#include <string>

namespace vanillapdf {
namespace syntax {

class TrackingIdentifierAttribute : public IAttribute {
public:
	TrackingIdentifierAttribute() = default;

	explicit TrackingIdentifierAttribute(const std::string& value) : m_value(value) {

	}

	std::string GetValue(void) const {
		return m_value;
	}

	virtual Type GetType(void) const noexcept override {
		return IAttribute::Type::TrackingIdentifier;
	}

	virtual TrackingIdentifierAttribute* Clone(void) const {
		return pdf_new TrackingIdentifierAttribute(m_value);
	}

private:
	std::string m_value;
};

} // syntax
} // vanillapdf

#endif /* _TRACKING_IDENTIFIER_ATTRIBUTE_H */
