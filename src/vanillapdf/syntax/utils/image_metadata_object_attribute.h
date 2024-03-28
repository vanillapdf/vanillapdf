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
	explicit ImageMetadataObjectAttribute(ColorSpaceType color_space, int32_t components)
		: m_color_space(color_space), m_components(components) {

	}

	ColorSpaceType GetColorSpace() const noexcept {
		return m_color_space;
	}

	int32_t GetColorComponents() const noexcept {
		return m_components;
	}

	virtual Type GetType(void) const noexcept override {
		return Type::ImageMetadata;
	}

	virtual ImageMetadataObjectAttribute* Clone(void) const override {
		return pdf_new ImageMetadataObjectAttribute(m_color_space, m_components);
	}

private:
	ColorSpaceType m_color_space = ColorSpaceType::Undefined;
	int32_t m_components = 0;
};

} // syntax
} // vanillapdf

#endif /* _IMAGE_METADATA_OBJECT_ATTRIBUTE_H */
