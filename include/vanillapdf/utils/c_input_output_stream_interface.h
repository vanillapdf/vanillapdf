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
	* \file c_input_output_stream_interface.h
	* \brief This file contains class definitions for \ref IInputOutputStreamHandle
	*/

	/**
	* \class IInputOutputStreamHandle
	* \extends IInputStreamHandle
	* \extends IOutputStreamHandle
	* \ingroup group_utils
	* \brief Input stream can read and interpret input from sequences of characters
	*/

	/**
	* \memberof IInputOutputStreamHandle
	* @{
	*/

	/**
	* \brief Opens an existing file at \p filename for read
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_CreateFromFile(string_type filename, IInputOutputStreamHandle** result);

	/**
	* \brief Reinterpret current object as \ref IInputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_ToInputStream(IInputOutputStreamHandle* handle, IInputStreamHandle** result);

	/**
	* \brief Reinterpret current object as \ref IOutputStreamHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_ToOutputStream(IInputOutputStreamHandle* handle, IOutputStreamHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_Release(IInputOutputStreamHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INPUT_OUTPUT_STREAM_INTERFACE_H */
