#ifndef _C_FILTER_H
#define _C_FILTER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_filter.h
	* This file contains encoding/decoding utilities
	*/

	/**
	* \class FilterBaseHandle
	* \extends IUnknownHandle
	* \ingroup Files
	* \brief TODO
	*/

	/**
	* \class FlateDecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup Files
	* \brief TODO
	*/

	/**
	* \class DCTDecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup Files
	* \brief TODO
	*/

	/**
	* \class ASCII85DecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup Files
	* \brief TODO
	*/

	/**
	* \class ASCIIHexDecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup Files
	* \brief TODO
	*/

	/**
	* \memberof FilterBaseHandle
	* @{
	*/

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_Release(FilterBaseHandle handle);

	/** @} */

	/**
	* \memberof FlateDecodeFilterHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Create(FlateDecodeFilterHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Encode(FlateDecodeFilterHandle handle, BufferHandle data, BufferHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Decode(FlateDecodeFilterHandle handle, BufferHandle data, BufferHandle* result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Release(FlateDecodeFilterHandle handle);

	/** @} */

	/**
	* \memberof DCTDecodeFilterHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Create(DCTDecodeFilterHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Encode(DCTDecodeFilterHandle handle, BufferHandle data, BufferHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Decode(DCTDecodeFilterHandle handle, BufferHandle data, BufferHandle* result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Release(DCTDecodeFilterHandle handle);

	/** @} */

	/**
	* \memberof ASCII85DecodeFilterHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Create(ASCII85DecodeFilterHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Encode(ASCII85DecodeFilterHandle handle, BufferHandle data, BufferHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Decode(ASCII85DecodeFilterHandle handle, BufferHandle data, BufferHandle* result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Release(ASCII85DecodeFilterHandle handle);

	/** @} */

	/**
	* \memberof ASCIIHexDecodeFilterHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Create(ASCIIHexDecodeFilterHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Encode(ASCIIHexDecodeFilterHandle handle, BufferHandle data, BufferHandle* result);
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Decode(ASCIIHexDecodeFilterHandle handle, BufferHandle data, BufferHandle* result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Release(ASCIIHexDecodeFilterHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILTER_H */
