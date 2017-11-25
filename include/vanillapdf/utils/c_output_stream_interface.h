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
	* This file contains class definitions for \ref OutputStreamInterfaceHandle
	*/

	/**
	* \class OutputStreamInterfaceHandle
	* \extends IUnknownHandle
	* \ingroup Utils
	* \brief \todo Brief description
	*/

	/**
	* \memberof OutputStreamInterfaceHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_CreateFromFile(string_type filename, OutputStreamInterfaceHandle** result);

	VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_WriteString(OutputStreamInterfaceHandle* handle, string_type data);
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_WriteBuffer(OutputStreamInterfaceHandle* handle, BufferHandle* data);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_Release(OutputStreamInterfaceHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OUTPUT_STREAM_INTERFACE_H */
