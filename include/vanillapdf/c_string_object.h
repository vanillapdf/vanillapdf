#ifndef _C_STRING_OBJECT_H
#define _C_STRING_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_string_object.h
	* This file contains class definitions for StringObjectHandle
	*/

	/**
	* \class StringObjectHandle
	* \extends ObjectHandle
	* \ingroup Objects
	* \brief Reprsents human readable text.
	*/

	/**
	* \class HexadecimalStringObjectHandle
	* \extends StringObjectHandle
	* \ingroup Objects
	* \brief Text stored in hexadecimal notation.
	*/

	/**
	* \class LiteralStringObjectHandle
	* \extends StringObjectHandle
	* \ingroup Objects
	* \brief Text stored in raw format.
	*/

	/**
	* \brief Available types of strings.
	*/
	typedef enum {
		/**
		* \copybrief LiteralStringObjectHandle
		* \see LiteralStringObjectHandle
		*/
		StringType_Literal = 0,

		/**
		* \copybrief HexadecimalStringObjectHandle
		* \see HexadecimalStringObjectHandle
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
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_Type(StringObjectHandle handle, StringType* result);

	/**
	* \brief Reinterpret current object as LiteralStringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_ToLiteral(StringObjectHandle handle, LiteralStringObjectHandle* result);

	/**
	* \brief Reinterpret current object as HexadecimalStringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_ToHexadecimal(StringObjectHandle handle, HexadecimalStringObjectHandle* result);

	/**
	* \brief Get strings current value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_GetValue(StringObjectHandle handle, BufferHandle* result);

	/**
	* \brief Set strings new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_SetValue(StringObjectHandle handle, BufferHandle value);

	/**
	* \copydoc Object_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION StringObject_Release(StringObjectHandle handle);

	/** @} */

	/**
	* \memberof LiteralStringObjectHandle
	* @{
	*/

	/**
	* \brief Get strings current value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_GetValue(LiteralStringObjectHandle handle, BufferHandle* result);

	/**
	* \brief Set strings new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_SetValue(LiteralStringObjectHandle handle, BufferHandle value);

	/**
	* \copydoc StringObject_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringObjectHandle handle);

	/** @} */

	/**
	* \memberof HexadecimalStringObjectHandle
	* @{
	*/

	/**
	* \brief Get strings current value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_GetValue(HexadecimalStringObjectHandle handle, BufferHandle* result);

	/**
	* \brief Set strings new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_SetValue(HexadecimalStringObjectHandle handle, BufferHandle value);

	/**
	* \copydoc StringObject_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringObjectHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_STRING_OBJECT_H */
