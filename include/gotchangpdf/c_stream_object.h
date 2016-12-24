#ifndef _C_STREAM_OBJECT_H
#define _C_STREAM_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_stream_object.h
	* This file contains class definitions for StreamObjectHandle
	*/

	/**
	* \brief Return streams header dictionary
	* \memberof StreamObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetHeader(StreamObjectHandle handle, PDictionaryObjectHandle result);

	/**
	* \brief Set new streams header dictionary
	* \memberof StreamObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_SetHeader(StreamObjectHandle handle, DictionaryObjectHandle value);

	/**
	* \brief Get uncompressed stream body
	*
	* This function is useful when the library does not understand the stream content.
	* \memberof StreamObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetBodyRaw(StreamObjectHandle handle, PBufferHandle result);

	/**
	* \brief Get decompressed stream content
	* \memberof StreamObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetBody(StreamObjectHandle handle, PBufferHandle result);

	/**
	* \brief Set new stream content
	*
	* This sets the decompressed content. Compression is done when the file will be saved.
	* \memberof StreamObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_SetBody(StreamObjectHandle handle, BufferHandle value);

	/**
	* \memberof StreamObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_Release(StreamObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_STREAM_OBJECT_H */
