#ifndef _IMAGE_METADATA_OBJECT_ATTRIBUTE_H
#define _IMAGE_METADATA_OBJECT_ATTRIBUTE_H

#include "syntax/utils/base_object_attribute.h"

namespace vanillapdf {
namespace syntax {

class ImageMetadataObjectAttribute : public BaseAttribute {
public:
    enum class ColorSpaceType {
        Undefined = 0,
        GRAY,
        RGB,
        CMYK
    };

public:
    ColorSpaceType GetColorSpace() const noexcept {
        return m_color_space;
    }

    void SetColorSpace(ColorSpaceType value) noexcept {
        m_color_space = value;
    }

    int32_t GetColorComponents() const noexcept {
        return m_components;
    }

    void SetColorComponents(int32_t value) noexcept {
        m_components = value;
    }

    int32_t GetWidth() const noexcept {
        return m_width;
    }

    void SetWidth(int32_t value) noexcept {
        m_width = value;
    }

    int32_t GetHeight() const noexcept {
        return m_height;
    }

    void SetHeight(int32_t value) noexcept {
        m_height = value;
    }

    virtual Type GetType(void) const noexcept override {
        return Type::ImageMetadata;
    }

    virtual ImageMetadataObjectAttribute* Clone(void) const override {
        auto result = pdf_new ImageMetadataObjectAttribute();

        result->SetWidth(m_width);
        result->SetHeight(m_height);
        result->SetColorSpace(m_color_space);
        result->SetColorComponents(m_components);

        return result;
    }

private:
    int32_t m_width = 0;
    int32_t m_height = 0;
    int32_t m_components = 0;
    ColorSpaceType m_color_space = ColorSpaceType::Undefined;
};

} // syntax
} // vanillapdf

#endif /* _IMAGE_METADATA_OBJECT_ATTRIBUTE_H */
