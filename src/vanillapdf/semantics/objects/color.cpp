#include "precompiled.h"

#include "semantics/objects/color.h"

namespace vanillapdf {
namespace semantics {

Color::Color(syntax::MixedArrayObjectPtr root) : HighLevelObject(root) {
    auto size = root->GetSize();

    switch (size) {
    case 0:
    case 1:
    case 3:
    case 4:
        break;
    default:
        throw InvalidParameterException("Invalid color array size: " + std::to_string(size));
    }
}

Color::ColorSpace Color::GetColorSpace() const {
    auto size = _obj->GetSize();

    switch (size) {
    case 0: return ColorSpace::Transparent;
    case 1: return ColorSpace::DeviceGray;
    case 3: return ColorSpace::DeviceRGB;
    case 4: return ColorSpace::DeviceCMYK;
    default:
        throw InvalidParameterException("Invalid color array size: " + std::to_string(size));
    }
}

double Color::GetComponent(types::size_type index) const {
    auto obj = _obj->GetValue(index);
    auto real = syntax::ObjectUtils::ConvertTo<syntax::RealObjectPtr>(obj);
    return real->GetValue();
}

void Color::SetComponent(types::size_type index, double value) {
    auto real = make_deferred<syntax::RealObject>(value, 3);
    _obj->SetValue(index, real);
}

// Grayscale

double Color::GetGray() const {
    if (GetColorSpace() != ColorSpace::DeviceGray) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(0);
}

void Color::SetGray(double value) {
    if (GetColorSpace() != ColorSpace::DeviceGray) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(0, value);
}

// RGB

double Color::GetRed() const {
    if (GetColorSpace() != ColorSpace::DeviceRGB) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(0);
}

void Color::SetRed(double value) {
    if (GetColorSpace() != ColorSpace::DeviceRGB) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(0, value);
}

double Color::GetGreen() const {
    if (GetColorSpace() != ColorSpace::DeviceRGB) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(1);
}

void Color::SetGreen(double value) {
    if (GetColorSpace() != ColorSpace::DeviceRGB) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(1, value);
}

double Color::GetBlue() const {
    if (GetColorSpace() != ColorSpace::DeviceRGB) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(2);
}

void Color::SetBlue(double value) {
    if (GetColorSpace() != ColorSpace::DeviceRGB) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(2, value);
}

// CMYK

double Color::GetCyan() const {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(0);
}

void Color::SetCyan(double value) {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(0, value);
}

double Color::GetMagenta() const {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(1);
}

void Color::SetMagenta(double value) {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(1, value);
}

double Color::GetYellow() const {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(2);
}

void Color::SetYellow(double value) {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(2, value);
}

double Color::GetBlack() const {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    return GetComponent(3);
}

void Color::SetBlack(double value) {
    if (GetColorSpace() != ColorSpace::DeviceCMYK) {
        throw NotSupportedException("Component not available for current color space");
    }
    SetComponent(3, value);
}

// Factory methods

ColorPtr Color::CreateTransparent() {
    syntax::MixedArrayObjectPtr arr;
    return make_deferred<Color>(arr);
}

ColorPtr Color::CreateGray(double gray) {
    syntax::MixedArrayObjectPtr arr;
    arr->Append(make_deferred<syntax::RealObject>(gray, 3));
    return make_deferred<Color>(arr);
}

ColorPtr Color::CreateRGB(double red, double green, double blue) {
    syntax::MixedArrayObjectPtr arr;
    arr->Append(make_deferred<syntax::RealObject>(red, 3));
    arr->Append(make_deferred<syntax::RealObject>(green, 3));
    arr->Append(make_deferred<syntax::RealObject>(blue, 3));
    return make_deferred<Color>(arr);
}

ColorPtr Color::CreateCMYK(double cyan, double magenta, double yellow, double black) {
    syntax::MixedArrayObjectPtr arr;
    arr->Append(make_deferred<syntax::RealObject>(cyan, 3));
    arr->Append(make_deferred<syntax::RealObject>(magenta, 3));
    arr->Append(make_deferred<syntax::RealObject>(yellow, 3));
    arr->Append(make_deferred<syntax::RealObject>(black, 3));
    return make_deferred<Color>(arr);
}

} // semantics
} // vanillapdf
