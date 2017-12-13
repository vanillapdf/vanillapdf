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
	* \file c_output_stream_interface.h
	* \brief This file contains class definitions for \ref IOutputStreamHandle
	*/

	/**
	* \class IOutputStreamHandle
	* \extends IUnknownHandle
	* \ingroup Utils
	* \brief Output stream can write sequences of characters and represent other kinds of data
	*/

	/**
	* \memberof IOutputStreamHandle
	* @{
	*/

	/**
	* \brief Creates a new file at \p filename location and opens it for writing
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_CreateFromFile(string_type filename, IOutputStreamHandle** result);

	/**
	* \brief Appends null terminated string to current output stream instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_WriteString(IOutputStreamHandle* handle, string_type data);

	/**
	* \brief Appends buffer string to current output stream instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_WriteBuffer(IOutputStreamHandle* handle, BufferHandle* data);

	/**
	* \brief Flushes all pending data from the stream to it's destination
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_Flush(IOutputStreamHandle* handle);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_Release(IOutputStreamHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OUTPUT_STREAM_INTERFACE_H */
