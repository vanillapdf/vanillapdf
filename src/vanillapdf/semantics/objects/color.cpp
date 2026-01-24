#include "precompiled.h"

#include "semantics/objects/color.h"

namespace vanillapdf {
namespace semantics {

Color::Color(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {
    auto size = root->GetSize();

    switch (size) {
    case 0:
        m_color_space = ColorSpace::Transparent;
        break;
    case 1:
        m_color_space = ColorSpace::DeviceGray;
        break;
    case 3:
        m_color_space = ColorSpace::DeviceRGB;
        break;
    case 4:
        m_color_space = ColorSpace::DeviceCMYK;
        break;
    default:
        throw GeneralException("Invalid color array size: " + std::to_string(size));
    }

    for (types::size_type i = 0; i < size && i < 4; ++i) {
        auto obj = root->GetValue(i);
        auto real = syntax::ObjectUtils::ConvertTo<syntax::RealObjectPtr>(obj);
        m_components[i] = real->GetValue();
    }
}

void Color::SetGray(double value) {
    m_color_space = ColorSpace::DeviceGray;
    m_components[0] = value;
    RebuildArray();
}

void Color::SetRed(double value) {
    m_components[0] = value;
    RebuildArray();
}

void Color::SetGreen(double value) {
    m_components[1] = value;
    RebuildArray();
}

void Color::SetBlue(double value) {
    m_components[2] = value;
    RebuildArray();
}

void Color::SetCyan(double value) {
    m_components[0] = value;
    RebuildArray();
}

void Color::SetMagenta(double value) {
    m_components[1] = value;
    RebuildArray();
}

void Color::SetYellow(double value) {
    m_components[2] = value;
    RebuildArray();
}

void Color::SetBlack(double value) {
    m_components[3] = value;
    RebuildArray();
}

void Color::RebuildArray() {
    // Clear and rebuild the underlying array
    while (_obj->GetSize() > 0) {
        _obj->Remove(0);
    }

    size_t count = 0;
    switch (m_color_space) {
    case ColorSpace::Transparent:
        count = 0;
        break;
    case ColorSpace::DeviceGray:
        count = 1;
        break;
    case ColorSpace::DeviceRGB:
        count = 3;
        break;
    case ColorSpace::DeviceCMYK:
        count = 4;
        break;
    }

    for (size_t i = 0; i < count; ++i) {
        _obj->Append(make_deferred<syntax::RealObject>(m_components[i], 3));
    }
}

ColorPtr Color::CreateTransparent() {
    syntax::MixedArrayObjectPtr arr;
    auto color = make_deferred<Color>(arr);
    color->m_color_space = ColorSpace::Transparent;
    return color;
}

ColorPtr Color::CreateGray(double gray) {
    syntax::MixedArrayObjectPtr arr;
    arr->Append(make_deferred<syntax::RealObject>(gray, 3));
    auto color = make_deferred<Color>(arr);
    return color;
}

ColorPtr Color::CreateRGB(double red, double green, double blue) {
    syntax::MixedArrayObjectPtr arr;
    arr->Append(make_deferred<syntax::RealObject>(red, 3));
    arr->Append(make_deferred<syntax::RealObject>(green, 3));
    arr->Append(make_deferred<syntax::RealObject>(blue, 3));
    auto color = make_deferred<Color>(arr);
    return color;
}

ColorPtr Color::CreateCMYK(double cyan, double magenta, double yellow, double black) {
    syntax::MixedArrayObjectPtr arr;
    arr->Append(make_deferred<syntax::RealObject>(cyan, 3));
    arr->Append(make_deferred<syntax::RealObject>(magenta, 3));
    arr->Append(make_deferred<syntax::RealObject>(yellow, 3));
    arr->Append(make_deferred<syntax::RealObject>(black, 3));
    auto color = make_deferred<Color>(arr);
    return color;
}

} // semantics
} // vanillapdf
