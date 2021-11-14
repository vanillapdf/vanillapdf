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
	* \brief This file contains contains class definitions of font ranges in CID fonts
	*/

	/**
	* \class BaseFontRangeHandle
	* \extends IUnknownHandle
	* \ingroup group_contents
	* \brief
	* Base font ranges are used for mapping input codes to
	* corresponding range of character codes or names
	*/

	/**
	* \memberof BaseFontRangeHandle
	* @{
	*/

	/**
	* \brief Creates a new empty instance of \ref BaseFontRangeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Create(BaseFontRangeHandle** result);

	/**
	* \brief Get the lower boundary of the font range
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetRangeLow(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle** result);

	/**
	* \brief Set the lower boundary of the font range
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetRangeLow(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle* data);

	/**
	* \brief Get the upper boundary of the font range
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetRangeHigh(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle** result);

	/**
	* \brief Set the upper boundary of the font range
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetRangeHigh(BaseFontRangeHandle* handle, HexadecimalStringObjectHandle* data);

	/**
	* \brief Get the mapped value that is associated with the range boundaries
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_GetDestination(BaseFontRangeHandle* handle, ObjectHandle** result);

	/**
	* \brief Set the mapped value that is associated with the range boundaries
	*
	* The data could be either \ref HexadecimalStringObjectHandle or \ref ArrayObjectHandle filled with one or multiple \ref HexadecimalStringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_SetDestination(BaseFontRangeHandle* handle, ObjectHandle* data);

	/**
	* \brief Check if parameter \p data is present in the mapping table
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseFontRange_Contains(BaseFontRangeHandle* handle, BufferHandle* data, boolean_type* result);

	/**
	* \brief Get corresponding value for parameter \p data in the mapping table
	*/
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
