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
	* \brief Reinterpret current object as \ref InputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToInputStream(InputOutputStreamHandle* handle, InputStreamHandle** result);

	/**
	* \brief Reinterpret current object as \ref OutputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToOutputStream(InputOutputStreamHandle* handle, OutputStreamHandle** result);

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
