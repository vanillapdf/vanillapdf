#ifndef _C_BUFFER_H
#define _C_BUFFER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_buffer.h
	* \brief This file contains class definitions for \ref BufferHandle
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
	* \brief Creates a new buffer instance with the content of \p data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Create(string_type data, size_type size, BufferHandle** result);

	/**
	* \brief Get data content from buffer
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Buffer_GetData(BufferHandle* handle, string_type* data, size_type* size);

	/**
	* \brief Set new data content
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Buffer_SetData(BufferHandle* handle, string_type data, size_type size);

	/**
	* \brief Converts current buffer state to input stream
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Buffer_ToInputStream(BufferHandle* handle, IInputStreamHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BUFFER_H */
