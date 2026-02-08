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

    ColorSpace GetColorSpace() const;

    // Grayscale
    double GetGray() const;
    void SetGray(double value);

    // RGB
    double GetRed() const;
    void SetRed(double value);

    double GetGreen() const;
    void SetGreen(double value);

    double GetBlue() const;
    void SetBlue(double value);

    // CMYK
    double GetCyan() const;
    void SetCyan(double value);

    double GetMagenta() const;
    void SetMagenta(double value);

    double GetYellow() const;
    void SetYellow(double value);

    double GetBlack() const;
    void SetBlack(double value);

    // Factory methods
    static ColorPtr CreateTransparent();
    static ColorPtr CreateGray(double gray);
    static ColorPtr CreateRGB(double red, double green, double blue);
    static ColorPtr CreateCMYK(double cyan, double magenta, double yellow, double black);

private:
    double GetComponent(types::size_type index) const;
    void SetComponent(types::size_type index, double value);
};

} // semantics
} // vanillapdf

#endif /* _COLOR_H */
