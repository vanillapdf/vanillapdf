#ifndef _C_INPUT_OUTPUT_STREAM_INTERFACE_H
#define _C_INPUT_OUTPUT_STREAM_INTERFACE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_input_output_stream.h
	* \brief This file contains class definitions for \ref InputOutputStreamHandle
	*/

	/**
	* \class InputOutputStreamHandle
	* \extends InputStreamHandle
	* \extends OutputStreamHandle
	* \extends IUnknownHandle
	* \ingroup group_utils
	* \brief Input stream can read and interpret input from sequences of characters
	*/

	/**
	* \memberof InputOutputStreamHandle
	* @{
	*/

	/**
	* \brief Opens an existing file at \p filename for read and write
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_CreateFromFile(string_type filename, InputOutputStreamHandle** result);

	/**
	* \brief Creates a memory buffer for data manipulation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_CreateFromMemory(InputOutputStreamHandle** result);

	/**
	* \copydoc InputStream_Read
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_Read(InputOutputStreamHandle* handle, bigint_type length, char* result, bigint_type* read_length);

	/**
	* \copydoc InputStream_ReadBuffer
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ReadBuffer(InputOutputStreamHandle* handle, bigint_type length, BufferHandle** result);

	/**
	* \copydoc InputStream_GetInputPosition
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_GetInputPosition(InputOutputStreamHandle* handle, offset_type* result);

	/**
	* \copydoc InputStream_SetInputPosition
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_SetInputPosition(InputOutputStreamHandle* handle, offset_type value);

	/**
	* \copydoc OutputStream_GetOutputPosition
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_GetOutputPosition(InputOutputStreamHandle* handle, offset_type* result);

	/**
	* \copydoc OutputStream_SetOutputPosition
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_SetOutputPosition(InputOutputStreamHandle* handle, offset_type value);

	/**
	* \copydoc OutputStream_WriteString
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_WriteString(InputOutputStreamHandle* handle, string_type data);

	/**
	* \copydoc OutputStream_WriteBuffer
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_WriteBuffer(InputOutputStreamHandle* handle, BufferHandle* data);

	/**
	* \copydoc OutputStream_Flush
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_Flush(InputOutputStreamHandle* handle);

	/**
	* \brief Reinterpret current object as \ref InputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToInputStream(InputOutputStreamHandle* handle, InputStreamHandle** result);

	/**
	* \brief Convert \ref InputStreamHandle to \ref InputOutputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_FromInputStream(InputStreamHandle* handle, InputOutputStreamHandle** result);

	/**
	* \brief Reinterpret current object as \ref OutputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToOutputStream(InputOutputStreamHandle* handle, OutputStreamHandle** result);

	/**
	* \brief Convert \ref InputStreamHandle to \ref InputOutputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_FromOutputStream(OutputStreamHandle* handle, InputOutputStreamHandle** result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToUnknown(InputOutputStreamHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref InputOutputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_FromUnknown(IUnknownHandle* handle, InputOutputStreamHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_Release(InputOutputStreamHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INPUT_OUTPUT_STREAM_INTERFACE_H */
