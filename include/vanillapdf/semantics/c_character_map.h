#ifndef _C_CHARACTER_MAP_H
#define _C_CHARACTER_MAP_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_character_map.h
	* \brief This file contains contains class definitions for character maps
	*/

	/**
	* \class CharacterMapHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief
	* A CMap shall specify the mapping from
	* character codes to character selectors.
	*/

	/**
	* \class EmbeddedCharacterMapHandle
	* \extends CharacterMapHandle
	* \ingroup group_documents
	* \brief
	* For character encodings that are not predefined,
	* the PDF file shall contain a stream that defines the CMap.
	*/

	/**
	* \class UnicodeCharacterMapHandle
	* \extends CharacterMapHandle
	* \ingroup group_documents
	* \brief
	* The Unicode standard defines a system for numbering all
	* of the common characters used in a large number of languages.
	*/

	/**
	* \brief Available types of character maps.
	*/
	typedef enum {
		/**
		* \copybrief EmbeddedCharacterMapHandle
		* \see \ref EmbeddedCharacterMapHandle
		*/
		CharacterMapType_Embedded = 0,

		/**
		* \copybrief UnicodeCharacterMapHandle
		* \see \ref UnicodeCharacterMapHandle
		*/
		CharacterMapType_Unicode
	} CharacterMapType;

	/**
	* \memberof CharacterMapHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_GetType(CharacterMapHandle* handle, CharacterMapType* result);

	/**
	* \brief Reinterpret current object as UnicodeCharacterMapHandle*
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_ToUnicode(CharacterMapHandle* handle, UnicodeCharacterMapHandle** result);

	/**
	* \brief Reinterpret current object as EmbeddedCharacterMapHandle*
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_ToEmbedded(CharacterMapHandle* handle, EmbeddedCharacterMapHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_Release(CharacterMapHandle* handle);

	/** @} */

	/**
	* \memberof UnicodeCharacterMapHandle
	* @{
	*/

	/**
	* \brief Get unicode representation of key \p key_handle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_GetMappedValue(
		UnicodeCharacterMapHandle* handle,
		BufferHandle* key_handle,
		BufferHandle** result);

	/**
	* \copydoc CharacterMap_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_Release(UnicodeCharacterMapHandle* handle);

	/** @} */

	/**
	* \memberof EmbeddedCharacterMapHandle
	* @{
	*/

	/**
	* \copydoc CharacterMap_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_Release(EmbeddedCharacterMapHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CHARACTER_MAP_H */
