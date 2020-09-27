#ifndef _C_STRING_OBJECT_H
#define _C_STRING_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_string_object.h
	* \brief This file contains class definitions for \ref StringObjectHandle
	*/

	/**
	* \class StringObjectHandle
	* \extends ObjectHandle
	* \ingroup group_objects
	* \brief Reprsents human readable text.
	*/

	/**
	* \class HexadecimalStringObjectHandle
	* \extends StringObjectHandle
	* \ingroup group_objects
	* \brief Text stored in hexadecimal notation.
	*/

	/**
	* \class LiteralStringObjectHandle
	* \extends StringObjectHandle
	* \ingroup group_objects
	* \brief Text stored in raw format.
	*/

	/**
	* \brief Available types of strings.
	*/
	typedef enum {
		/**
		* \copybrief LiteralStringObjectHandle
		* \see \ref LiteralStringObjectHandle
		*/
		StringType_Literal = 0,

		/**
		* \copybrief HexadecimalStringObjectHandle
		* \see \ref HexadecimalStringObjectHandle
		*/
		StringType_Hexadecimal
	} StringType;

	/**
	* \memberof StringObjectHandle
	* @{
	*/

	/**
	* \brief Get derived type from current string
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_GetType(StringObjectHandle* handle, StringType* result);

	/**
	* \brief Get strings current value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_GetValue(StringObjectHandle* handle, BufferHandle** result);

	/**
	* \brief Set strings new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_SetValue(StringObjectHandle* handle, BufferHandle* value);

	/**
	* \brief Reinterpret current object as \ref ObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_ToObject(StringObjectHandle* handle, ObjectHandle** result);

	/**
	* \brief Convert \ref ObjectHandle to \ref StringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_FromObject(ObjectHandle* handle, StringObjectHandle** result);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_Release(StringObjectHandle* handle);

	/** @} */

	/**
	* \memberof LiteralStringObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new LiteralStringObject instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_Create(LiteralStringObjectHandle** result);

	/**
	* \brief Creates a new LiteralStringObject instance from encoded data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_CreateFromEncodedBuffer(BufferHandle* value, LiteralStringObjectHandle** result);

		/**
	* \brief Creates a new LiteralStringObject instance from encoded string data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_CreateFromEncodedString(string_type value, LiteralStringObjectHandle** result);

	/**
	* \brief Creates a new LiteralStringObject instance from decoded data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_CreateFromDecodedBuffer(BufferHandle* value, LiteralStringObjectHandle** result);

		/**
	* \brief Creates a new LiteralStringObject instance from decoded string data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_CreateFromDecodedString(string_type value, LiteralStringObjectHandle** result);

	/**
	* \brief Get strings current value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_GetValue(LiteralStringObjectHandle* handle, BufferHandle** result);

	/**
	* \brief Set strings new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_SetValue(LiteralStringObjectHandle* handle, BufferHandle* value);

	/**
	* \brief Reinterpret current object as \ref StringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_ToStringObject(LiteralStringObjectHandle* handle, StringObjectHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref LiteralStringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_FromStringObject(StringObjectHandle* handle, LiteralStringObjectHandle** result);

	/**
	* \copydoc StringObject_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringObjectHandle* handle);

	/** @} */

	/**
	* \memberof HexadecimalStringObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new HexadecimalStringObject instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Create(HexadecimalStringObjectHandle** result);

	/**
	* \brief Creates a new HexadecimalStringObject instance from encoded data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_CreateFromEncodedBuffer(BufferHandle* value, HexadecimalStringObjectHandle** result);

		/**
	* \brief Creates a new HexadecimalStringObject instance from encoded string data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_CreateFromEncodedString(string_type value, HexadecimalStringObjectHandle** result);

		/**
	* \brief Creates a new HexadecimalStringObject instance from decoded data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_CreateFromDecodedBuffer(BufferHandle* value, HexadecimalStringObjectHandle** result);

		/**
	* \brief Creates a new HexadecimalStringObject instance from decoded string data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_CreateFromDecodedString(string_type value, HexadecimalStringObjectHandle** result);

	/**
	* \brief Get strings current value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_GetValue(HexadecimalStringObjectHandle* handle, BufferHandle** result);

	/**
	* \brief Set strings new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_SetValue(HexadecimalStringObjectHandle* handle, BufferHandle* value);

	/**
	* \brief Reinterpret current object as \ref StringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_ToStringObject(HexadecimalStringObjectHandle* handle, StringObjectHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref HexadecimalStringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_FromStringObject(StringObjectHandle* handle, HexadecimalStringObjectHandle** result);

	/**
	* \copydoc StringObject_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_STRING_OBJECT_H */
