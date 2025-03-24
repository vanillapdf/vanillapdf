#ifndef _OBJECT_STREAM_METADATA_ATTRIBUTE_H
#define _OBJECT_STREAM_METADATA_ATTRIBUTE_H

#include "syntax/utils/base_object_attribute.h"

namespace vanillapdf {
namespace syntax {

class ObjectStreamMetadataAttribute : public BaseAttribute {

public:
    bool GetInitialized() const noexcept {
        return m_initialized;
    }

    void SetInitialized(bool value) noexcept {
        m_initialized = value;
    }

    virtual Type GetType(void) const noexcept override {
        return Type::ObjectStreamMetadata;
    }

    virtual ObjectStreamMetadataAttribute* Clone(void) const override {
        auto result = pdf_new ObjectStreamMetadataAttribute();

        result->SetInitialized(m_initialized);

        return result;
    }

private:
    bool m_initialized = false;
};

} // syntax
} // vanillapdf

#endif /* _OBJECT_STREAM_METADATA_ATTRIBUTE_H */
