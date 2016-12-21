#ifndef _C_STRING_OBJECT_H
#define _C_STRING_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_string_object.h
	* This file contains contains class definitions for StringObjectHandle
	*/

	/**
	* \brief Available types of strings.
	*/
	typedef enum
	{
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
	* \brief Pointer to StringType
	*/
	typedef StringType *PStringType;

	/**
	* \brief Get derived type from current string
	* \memberof StringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Type(StringObjectHandle handle, PStringType result);

	/**
	* \brief Reinterpret current object as LiteralStringObjectHandle
	* \memberof StringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToLiteral(StringObjectHandle handle, PLiteralStringObjectHandle result);

	/**
	* \brief Reinterpret current object as HexadecimalStringObjectHandle
	* \memberof StringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToHexadecimal(StringObjectHandle handle, PHexadecimalStringObjectHandle result);

	/**
	* \brief Get strings current value
	* \memberof StringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_GetValue(StringObjectHandle handle, PBufferHandle result);

	/**
	* \brief Set strings new value
	* \memberof StringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_SetValue(StringObjectHandle handle, BufferHandle value);

	/**
	* \memberof StringObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Release(StringObjectHandle handle);

	/**
	* \brief Get strings current value
	* \memberof LiteralStringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_GetValue(LiteralStringObjectHandle handle, PBufferHandle result);

	/**
	* \brief Set strings new value
	* \memberof LiteralStringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_SetValue(LiteralStringObjectHandle handle, BufferHandle value);

	/**
	* \memberof LiteralStringObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringObjectHandle handle);

	/**
	* \brief Get strings current value
	* \memberof HexadecimalStringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_GetValue(HexadecimalStringObjectHandle handle, PBufferHandle result);

	/**
	* \brief Set strings new value
	* \memberof HexadecimalStringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_SetValue(HexadecimalStringObjectHandle handle, BufferHandle value);

	/**
	* \memberof HexadecimalStringObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_STRING_OBJECT_H */
