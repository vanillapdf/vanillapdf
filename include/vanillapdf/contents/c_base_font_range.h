#ifndef _C_BASE_FONT_RANGE_H
#define _C_BASE_FONT_RANGE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_base_font_range.h
	* \brief This file contains contains class definitions for character maps
	*/

	/**
	* \class BaseFontRangeHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief
	* A CMap shall specify the mapping from
	* character codes to character selectors.
	*/

	/**
	* \memberof BaseFontRangeHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Create(BaseFontRangeHandle** result);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetRangeLow(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle** result);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetRangeLow(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle* data);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetRangeHigh(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle** result);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetRangeHigh(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle* data);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetDestination(BaseFontRangeHandle* handle, ObjectHandle** result);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetDestination(BaseFontRangeHandle* handle, ObjectHandle* data);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Contains(BaseFontRangeHandle* handle, BufferHandle* data, boolean_type* result);
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetMappedValue(BaseFontRangeHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_ToUnknown(BaseFontRangeHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref BaseFontRangeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_FromUnknown(IUnknownHandle* handle, BaseFontRangeHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Release(BaseFontRangeHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BASE_FONT_RANGE_H */
