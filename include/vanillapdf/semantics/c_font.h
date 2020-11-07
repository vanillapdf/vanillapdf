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
	* \brief This file contains class definitions for \ref FontHandle and \ref CompositeFontHandle.
	*/

	/**
	* \class FontHandle
	* \extends IUnknownHandle
	* \ingroup group_fonts
	* \brief
	* Base class for all fonts in to representable inside a PDF document
	*
	* For more details please visit [section 9.5 - Introduction to Font Data Structures](PDF32000_2008.pdf#G8.1694788).
	*/

	/**
	* \class CompositeFontHandle
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
		* \copybrief CompositeFontHandle
		* \see \ref CompositeFontHandle
		*/
		FontType_Composite,

		/**
		* \brief
		* A font that defines glyph shapes using Type 1 font technology.
		*/
		FontType_Type1,

		/**
		* \brief
		* A font that defines glyphs with streams of PDF graphics operators.
		*/
		FontType_Type3
	} FontType;

	/**
	* \memberof FontHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Font_GetFontType(FontHandle* handle, FontType* result);

	/**
	* \brief Reinterpret current object as \ref CompositeFontHandle.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Font_ToComposite(FontHandle* handle, CompositeFontHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Font_Release(FontHandle* handle);

	/** @} */

	/**
	* \memberof CompositeFontHandle
	* @{
	*/

	/**
	* \brief
	* A stream containing a CMap file that maps character codes
	* to Unicode values (see 9.10, "Extraction of Text Content").
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CompositeFont_GetUnicodeMap(CompositeFontHandle* handle, UnicodeCharacterMapHandle** result);

	/**
	* \copydoc Font_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CompositeFont_Release(CompositeFontHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_H */
