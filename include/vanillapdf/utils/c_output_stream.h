#ifndef _C_OUTPUT_STREAM_INTERFACE_H
#define _C_OUTPUT_STREAM_INTERFACE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_output_stream.h
	* \brief This file contains class definitions for \ref OutputStreamHandle
	*/

	/**
	* \class OutputStreamHandle
	* \extends IUnknownHandle
	* \ingroup group_utils
	* \brief Output stream can write sequences of characters and represent other kinds of data
	*/

	/**
	* \memberof OutputStreamHandle
	* @{
	*/

	/**
	* \brief Creates a new file at \p filename location and opens it for writing
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_CreateFromFile(string_type filename, OutputStreamHandle** result);

	/**
	* \brief Gets current offset in the output stream
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_GetOutputPosition(OutputStreamHandle* handle, offset_type* result);

	/**
	* \brief Sets current offset in the output stream
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_SetOutputPosition(OutputStreamHandle* handle, offset_type value);

	/**
	* \brief Appends null terminated string data to current output stream instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_WriteString(OutputStreamHandle* handle, string_type data);

	/**
	* \brief Appends buffer data to current output stream instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_WriteBuffer(OutputStreamHandle* handle, BufferHandle* data);

	/**
	* \brief Flushes all pending data from the stream to it's destination
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_Flush(OutputStreamHandle* handle);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_ToUnknown(OutputStreamHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref OutputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_FromUnknown(IUnknownHandle* handle, OutputStreamHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_Release(OutputStreamHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OUTPUT_STREAM_INTERFACE_H */
