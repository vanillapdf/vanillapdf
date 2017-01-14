#ifndef _C_CHARACTER_MAP_H
#define _C_CHARACTER_MAP_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_character_map.h
	* This file contains contains class definitions for character maps
	*/

	/**
	* \class CharacterMapHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* A CMap shall specify the mapping from
	* character codes to character selectors.
	*/

	/**
	* \class EmbeddedCharacterMapHandle
	* \extends CharacterMapHandle
	* \ingroup Documents
	* \brief
	* For character encodings that are not predefined,
	* the PDF file shall contain a stream that defines the CMap.
	*/

	/**
	* \class UnicodeCharacterMapHandle
	* \extends CharacterMapHandle
	* \ingroup Documents
	* \brief
	* The Unicode standard defines a system for numbering all
	* of the common characters used in a large number of languages.
	*/

	/**
	* \brief Available types of character maps.
	*/
	typedef enum
	{
		/**
		* \copybrief EmbeddedCharacterMapHandle
		* \see EmbeddedCharacterMapHandle
		*/
		CharacterMapType_Embedded = 0,

		/**
		* \copybrief UnicodeCharacterMapHandle
		* \see UnicodeCharacterMapHandle
		*/
		CharacterMapType_Unicode
	} CharacterMapType;

	/**
	* \brief Pointer to CharacterMapType
	*/
	typedef CharacterMapType *PCharacterMapType;

	/**
	* \memberof CharacterMapHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_GetType(CharacterMapHandle handle, PCharacterMapType result);

	/**
	* \brief Reinterpret current object as UnicodeCharacterMapHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToUnicode(CharacterMapHandle handle, UnicodeCharacterMapHandle* result);

	/**
	* \brief Reinterpret current object as EmbeddedCharacterMapHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToEmbedded(CharacterMapHandle handle, EmbeddedCharacterMapHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_Release(CharacterMapHandle handle);

	/** @} */

	/**
	* \memberof UnicodeCharacterMapHandle
	* @{
	*/

	/**
	* \brief Get unicode representation of key \p key_handle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_GetMappedValue(
		UnicodeCharacterMapHandle handle,
		BufferHandle key_handle,
		BufferHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_Release(UnicodeCharacterMapHandle handle);

	/** @} */

	/**
	* \memberof EmbeddedCharacterMapHandle
	* @{
	*/

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_Release(EmbeddedCharacterMapHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CHARACTER_MAP_H */
