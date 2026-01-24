#ifndef _COLOR_H
#define _COLOR_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

namespace vanillapdf {
namespace semantics {

/**
 * Represents a color value that can be used for annotation colors.
 * Supports DeviceGray (1 component), DeviceRGB (3 components),
 * and DeviceCMYK (4 components) color spaces.
 */
class Color : public HighLevelObject<syntax::MixedArrayObjectPtr> {
public:
    enum class ColorSpace {
        Transparent,  // Empty array - no color
        DeviceGray,   // 1 component
        DeviceRGB,    // 3 components
        DeviceCMYK    // 4 components
    };

public:
    Color() = default;
    explicit Color(syntax::MixedArrayObjectPtr root);

    ColorSpace GetColorSpace() const noexcept { return m_color_space; }

    // Grayscale
    double GetGray() const noexcept { return m_components[0]; }
    void SetGray(double value);

    // RGB
    double GetRed() const noexcept { return m_components[0]; }
    void SetRed(double value);

    double GetGreen() const noexcept { return m_components[1]; }
    void SetGreen(double value);

    double GetBlue() const noexcept { return m_components[2]; }
    void SetBlue(double value);

    // CMYK
    double GetCyan() const noexcept { return m_components[0]; }
    void SetCyan(double value);

    double GetMagenta() const noexcept { return m_components[1]; }
    void SetMagenta(double value);

    double GetYellow() const noexcept { return m_components[2]; }
    void SetYellow(double value);

    double GetBlack() const noexcept { return m_components[3]; }
    void SetBlack(double value);

    // Factory methods
    static ColorPtr CreateTransparent();
    static ColorPtr CreateGray(double gray);
    static ColorPtr CreateRGB(double red, double green, double blue);
    static ColorPtr CreateCMYK(double cyan, double magenta, double yellow, double black);

private:
    void RebuildArray();

    ColorSpace m_color_space = ColorSpace::Transparent;
    double m_components[4] = {0.0, 0.0, 0.0, 0.0};
};

} // semantics
} // vanillapdf

#endif /* _COLOR_H */
