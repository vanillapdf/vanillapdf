#ifndef _C_INPUT_STREAM_INTERFACE_H
#define _C_INPUT_STREAM_INTERFACE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_streams.h
	* This file contains class definitions for InputStreamInterfaceHandle
	*/

	/**
	* \class InputStreamInterfaceHandle
	* \extends IUnknown
	* \ingroup Utils
	* \brief TODO
	*/

	/**
	* \memberof InputStreamInterfaceHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION InputStreamInterface_CreateFromFile(string_type filename, InputStreamInterfaceHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION InputStreamInterface_CreateFromBuffer(BufferHandle data, InputStreamInterfaceHandle* result);

	VANILLAPDF_API error_type CALLING_CONVENTION InputStreamInterface_ToBuffer(InputStreamInterfaceHandle handle, BufferHandle* result);

	/**
	* \copydoc IUnknown_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION InputStreamInterface_Release(InputStreamInterfaceHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INPUT_STREAM_INTERFACE_H */
