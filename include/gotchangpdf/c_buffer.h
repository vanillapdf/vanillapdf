#ifndef _C_BUFFER_H
#define _C_BUFFER_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_buffer.h
	* This file contains class definitions for BufferHandle
	*/

	/**
	* \class BufferHandle
	* \extends IUnknownHandle
	* \ingroup Utils
	* \brief Represents memory stored data
	*/

	/**
	* \memberof BufferHandle
	* @{
	*/

	/**
	* \brief Get data content from buffer
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_GetData(BufferHandle handle, string_type* data, size_type* size);

	/**
	* \brief Set new data content
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_SetData(BufferHandle handle, string_type data, size_type size);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BUFFER_H */
