#ifndef _C_BUFFER_ARRAY_H
#define _C_BUFFER_ARRAY_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_buffer_array.h
	* \brief This file contains class definitions for \ref BufferArrayHandle
	*/

	/**
	* \class BufferArrayHandle
	* \extends IUnknownHandle
	* \ingroup group_utils
	* \brief Represents a list of /ref BufferHandle
	*/

	/**
	* \memberof BufferArrayHandle
	* @{
	*/

	/**
	* \brief Return size of an array
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Size(BufferArrayHandle* handle, size_type* result);

	/**
	* \brief Get element at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_At(BufferArrayHandle* handle, size_type at, BufferHandle** result);

	/**
	* \brief Insert new element at the end of the array
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Append(BufferArrayHandle* handle, BufferHandle* value);

	/**
	* \brief Insert new element at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Insert(BufferArrayHandle* handle, size_type at, BufferHandle* value);

	/**
	* \brief Remove element from location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Remove(BufferArrayHandle* handle, size_type at);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Release(BufferArrayHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BUFFER_ARRAY_H */
