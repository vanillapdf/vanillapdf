#ifndef _C_INPUT_STREAM_INTERFACE_H
#define _C_INPUT_STREAM_INTERFACE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_input_stream.h
	* \brief This file contains class definitions for \ref InputStreamHandle
	*/

	/**
	* \class InputStreamHandle
	* \extends IUnknownHandle
	* \ingroup group_utils
	* \brief Input stream can read and interpret input from sequences of characters
	*/

	/**
	* \memberof InputStreamHandle
	* @{
	*/

	/**
	* \brief Opens an existing file at \p filename for read
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputStream_CreateFromFile(string_type filename, InputStreamHandle** result);

	/**
	* \brief Reads all data from buffer and stores them for later use
	* \details This method does not reflect buffer updates after it has been called.
	* Only current snapshot of the buffer state is stored in the stream.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputStream_CreateFromBuffer(BufferHandle* data, InputStreamHandle** result);

	/**
	* \brief Reads all data from the input stream and returns them as a single large buffer.
	* \details This method is not recommended for large files, as the process might not have enough memory.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputStream_ToBuffer(InputStreamHandle* handle, BufferHandle** result);

	/**
	* \brief Gets current offset in the input stream
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputStream_GetInputPosition(InputStreamHandle* handle, offset_type* result);

	/**
	* \brief Sets current offset in the input stream
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputStream_SetInputPosition(InputStreamHandle* handle, offset_type value);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputStream_Release(InputStreamHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INPUT_STREAM_INTERFACE_H */
