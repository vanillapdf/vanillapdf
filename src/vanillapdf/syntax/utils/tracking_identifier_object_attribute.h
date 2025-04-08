#ifndef _TRACKING_IDENTIFIER_ATTRIBUTE_H
#define _TRACKING_IDENTIFIER_ATTRIBUTE_H

#include "syntax/utils/base_object_attribute.h"

#include <string>

namespace vanillapdf {
namespace syntax {

class TrackingIdentifierAttribute : public BaseAttribute {
public:
    TrackingIdentifierAttribute() = default;

    explicit TrackingIdentifierAttribute(const std::string& value) : m_value(value) {

    }

    std::string GetValue(void) const {
        return m_value;
    }

    virtual Type GetType(void) const noexcept override {
        return BaseAttribute::Type::TrackingIdentifier;
    }

    virtual TrackingIdentifierAttribute* Clone(void) const override {
        return pdf_new TrackingIdentifierAttribute(m_value);
    }

private:
    std::string m_value;
};

} // syntax
} // vanillapdf

#endif /* _TRACKING_IDENTIFIER_ATTRIBUTE_H */
