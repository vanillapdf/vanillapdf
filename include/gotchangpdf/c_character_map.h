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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToUnicode(CharacterMapHandle handle, PUnicodeCharacterMapHandle result);

	/**
	* \brief Reinterpret current object as EmbeddedCharacterMapHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION CharacterMap_ToEmbedded(CharacterMapHandle handle, PEmbeddedCharacterMapHandle result);

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
		PBufferHandle result);

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
