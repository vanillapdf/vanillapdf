#ifndef _C_FONT_H
#define _C_FONT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_font.h
	* This file contains class definitions for
	* FontHandle and CompositeFontHandle.
	*/

	/**
	* \class FontHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* A base class for all derived fonts.
	* \see FontMapHandle
	*/

	/**
	* \class CompositeFontHandle
	* \extends FontHandle
	* \ingroup Documents
	* \brief
	* A font composed of glyphs from a descendant
	* CIDFont (see 9.7, "Composite Fonts").
	*/

	/**
	* \brief Types of fonts
	*/
	typedef enum
	{
		/**
		* \copydoc CompositeFontHandle
		* \see CompositeFontHandle
		*/
		FontType_Composite = 0,

		/**
		* \brief
		* A font that defines glyph shapes using Type 1
		* font technology (see 9.6.2, "Type 1 Fonts").
		*/
		FontType_Type1,

		/**
		* \brief
		* A font that defines glyphs with streams of PDF
		* graphics operators (see 9.6.5, "Type 3 Fonts").
		*/
		FontType_Type3
	} FontType;

	/**
	* \brief Pointer to FontType
	*/
	typedef FontType *PFontType;

	/**
	* \memberof FontHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Font_Type(FontHandle handle, PFontType result);

	/**
	* \brief Reinterpret current object as CompositeFontHandle.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Font_ToComposite(FontHandle handle, PCompositeFontHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Font_Release(FontHandle handle);

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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CompositeFont_GetUnicodeMap(CompositeFontHandle handle, PUnicodeCharacterMapHandle result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CompositeFont_Release(CompositeFontHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_H */
