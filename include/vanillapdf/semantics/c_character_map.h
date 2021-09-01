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
		* \brief Undefined unitialized default value, triggers error when used
		*/
		CharacterMapType_Undefined = 0,

		/**
		* \copybrief EmbeddedCharacterMapHandle
		* \see \ref EmbeddedCharacterMapHandle
		*/
		CharacterMapType_Embedded,

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
	VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_GetCharacterMapType(CharacterMapHandle* handle, CharacterMapType* result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_ToUnknown(CharacterMapHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref FontHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION CharacterMap_FromUnknown(IUnknownHandle* handle, CharacterMapHandle** result);

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
	* \brief Reinterpret current object as \ref CharacterMapHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_ToCharacterMap(UnicodeCharacterMapHandle* handle, CharacterMapHandle** result);

	/**
	* \brief Convert \ref CharacterMapHandle to \ref UnicodeCharacterMapHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION UnicodeCharacterMap_FromCharacterMap(CharacterMapHandle* handle, UnicodeCharacterMapHandle** result);

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
	* \brief Reinterpret current object as \ref CharacterMapHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_ToCharacterMap(EmbeddedCharacterMapHandle* handle, CharacterMapHandle** result);

	/**
	* \brief Convert \ref CharacterMapHandle to \ref EmbeddedCharacterMapHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_FromCharacterMap(CharacterMapHandle* handle, EmbeddedCharacterMapHandle** result);

	/**
	* \copydoc CharacterMap_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION EmbeddedCharacterMap_Release(EmbeddedCharacterMapHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CHARACTER_MAP_H */
