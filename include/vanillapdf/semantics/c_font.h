#ifndef _C_FONT_H
#define _C_FONT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_font.h
    * \brief This file contains class definitions for \ref FontHandle and \ref Type0FontHandle.
    */

    /**
    * \class FontHandle
    * \extends IUnknownHandle
    * \ingroup group_fonts
    * \brief
    * Base class for all fonts in to representable inside a PDF document.
    *
    * For more details please visit [section 9.5 - Introduction to Font Data Structures](PDF32000_2008.pdf#G8.1694788).
    */

    /**
    * \class Type0FontHandle
    * \extends FontHandle
    * \ingroup group_fonts
    * \brief
    * A font composed of glyphs from a descendant CIDFont.
    *
    * For more details please visit [section 9.7 - Composite Fonts](PDF32000_2008.pdf#G8.1851619).
    */

    /**
    * \brief Types of fonts
    * \ingroup group_fonts
    */
    typedef enum {
        FontType_Undefined = 0,

        /**
        * \copybrief Type0FontHandle
        * \see \ref Type0FontHandle
        */
        FontType_Type0,

        /**
        * \brief
        * A font that defines glyph shapes using Type 1 font technology.
        */
        FontType_Type1,
        FontType_MMType1,

        /**
        * \brief
        * A font that defines glyphs with streams of PDF graphics operators.
        */
        FontType_Type3,
        FontType_TrueType,
        FontType_CIDFontType0,
        FontType_CIDFontType2
    } FontType;

    /**
    * \memberof FontHandle
    * @{
    */

    /**
    * \brief Create new font object from the specified dictionary
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_CreateFromObject(DictionaryObjectHandle* handle, FontHandle** result);

    /**
    * \brief Get derived type of current object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_GetFontType(FontHandle* handle, FontType* result);

    /**
    * \brief
    * A stream containing a CMap file that maps character codes
    * to Unicode values (see 9.10, "Extraction of Text Content").
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_GetUnicodeMap(FontHandle* handle, UnicodeCharacterMapHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_ToUnknown(FontHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref FontHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_FromUnknown(IUnknownHandle* handle, FontHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_Release(FontHandle* handle);

    /** @} */

    /**
    * \memberof Type0FontHandle
    * @{
    */

    /**
    * \brief Reinterpret current object as \ref FontHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Type0Font_ToFont(Type0FontHandle* handle, FontHandle** result);

    /**
    * \brief Convert \ref ObjectHandle to \ref Type0FontHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Type0Font_FromFont(FontHandle* handle, Type0FontHandle** result);

    /**
    * \copydoc Font_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Type0Font_Release(Type0FontHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_H */
