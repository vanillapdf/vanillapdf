#ifndef _IMAGE_COLOR_COMPONENTS_OBJECT_ATTRIBUTE_H
#define _IMAGE_COLOR_COMPONENTS_OBJECT_ATTRIBUTE_H

#include "syntax/utils/base_object_attribute.h"

namespace vanillapdf {
namespace syntax {

class ImageColorComponentsObjectAttribute : public BaseAttribute {
public:
	explicit ImageColorComponentsObjectAttribute(int32_t components) : m_components(components) {

	}

	virtual Type GetType(void) const noexcept override {
		return Type::ImageColorComponents;
	}

	virtual ImageColorComponentsObjectAttribute* Clone(void) const override {
		return pdf_new ImageColorComponentsObjectAttribute(m_components);
	}

private:
	int32_t m_components = 0;
};

} // syntax
} // vanillapdf

#endif /* _IMAGE_COLOR_COMPONENTS_OBJECT_ATTRIBUTE_H */
