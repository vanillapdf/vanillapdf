#ifndef _IMAGE_COLOR_SPACE_OBJECT_ATTRIBUTE_H
#define _IMAGE_COLOR_SPACE_OBJECT_ATTRIBUTE_H

#include "syntax/utils/base_object_attribute.h"

namespace vanillapdf {
namespace syntax {

class ImageColorSpaceObjectAttribute : public BaseAttribute {
public:
	explicit ImageColorSpaceObjectAttribute(int32_t color_space) : m_color_space(color_space) {

	}

	virtual Type GetType(void) const noexcept override {
		return Type::ImageColorSpace;
	}

	virtual ImageColorSpaceObjectAttribute* Clone(void) const override {
		return pdf_new ImageColorSpaceObjectAttribute(m_color_space);
	}

private:
	int32_t m_color_space = 0;
};

} // syntax
} // vanillapdf

#endif /* _IMAGE_COLOR_SPACE_OBJECT_ATTRIBUTE_H */
