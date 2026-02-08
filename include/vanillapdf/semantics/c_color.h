#ifndef _C_COLOR_H
#define _C_COLOR_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_color.h
    * \brief This file contains class definitions for \ref ColorHandle
    */

    /**
    * \class ColorHandle
    * \extends IUnknownHandle
    * \ingroup group_common_data_structures
    * \brief Represents a color value for annotations and other PDF elements
    */

    /**
    * \brief Color space types
    */
    typedef enum {
        /**
        * \brief Transparent (no color)
        */
        ColorSpace_Transparent = 0,

        /**
        * \brief DeviceGray color space (1 component)
        */
        ColorSpace_DeviceGray,

        /**
        * \brief DeviceRGB color space (3 components)
        */
        ColorSpace_DeviceRGB,

        /**
        * \brief DeviceCMYK color space (4 components)
        */
        ColorSpace_DeviceCMYK
    } ColorSpaceType;

    /**
    * \memberof ColorHandle
    * @{
    */

    /**
    * \brief Create a transparent color (no color)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateTransparent(ColorHandle** result);

    /**
    * \brief Create a grayscale color
    * \param gray the gray component (0.0 to 1.0)
    * \param result a pointer to variable that will contain the new color upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateGray(real_type gray, ColorHandle** result);

    /**
    * \brief Create an RGB color
    * \param red the red component (0.0 to 1.0)
    * \param green the green component (0.0 to 1.0)
    * \param blue the blue component (0.0 to 1.0)
    * \param result a pointer to variable that will contain the new color upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateRGB(real_type red, real_type green, real_type blue, ColorHandle** result);

    /**
    * \brief Create a CMYK color
    * \param cyan the cyan component (0.0 to 1.0)
    * \param magenta the magenta component (0.0 to 1.0)
    * \param yellow the yellow component (0.0 to 1.0)
    * \param black the black component (0.0 to 1.0)
    * \param result a pointer to variable that will contain the new color upon success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateCMYK(real_type cyan, real_type magenta, real_type yellow, real_type black, ColorHandle** result);

    /**
    * \brief Get the color space type
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetColorSpace(ColorHandle* handle, ColorSpaceType* result);

    /**
    * \brief Get the red component (for RGB colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetRed(ColorHandle* handle, real_type* result);

    /**
    * \brief Get the green component (for RGB colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetGreen(ColorHandle* handle, real_type* result);

    /**
    * \brief Get the blue component (for RGB colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetBlue(ColorHandle* handle, real_type* result);

    /**
    * \brief Get the gray component (for grayscale colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetGray(ColorHandle* handle, real_type* result);

    /**
    * \brief Get the cyan component (for CMYK colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetCyan(ColorHandle* handle, real_type* result);

    /**
    * \brief Get the magenta component (for CMYK colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetMagenta(ColorHandle* handle, real_type* result);

    /**
    * \brief Get the yellow component (for CMYK colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetYellow(ColorHandle* handle, real_type* result);

    /**
    * \brief Get the black component (for CMYK colors)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_GetBlack(ColorHandle* handle, real_type* result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_ToUnknown(ColorHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref ColorHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_FromUnknown(IUnknownHandle* handle, ColorHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Color_Release(ColorHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_COLOR_H */
