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
	* \brief This file contains encoding/decoding utilities
	*/

	/**
	* \class FilterBaseHandle
	* \extends IUnknownHandle
	* \ingroup group_files
	* \brief Base class for all compression filters
	*/

	/**
	* \class FlateDecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup group_files
	* \brief The Flate method is based on the public-domain zlib/deflate compression method
	* \details It is fully defined in Internet [RFC 1950 - ZLIB Compressed Data Format Specification](https://www.ietf.org/rfc/rfc1950.txt),
	* and [RFC 1951 DEFLATE Compressed Data Format Specification](https://www.ietf.org/rfc/rfc1951.txt).<br>
	* For more information please visit [section 7.4.4 - LZWDecode and FlateDecode Filters](PDF32000_2008.pdf#G6.1639121)
	*/

	/**
	* \class DCTDecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup group_files
	* \brief The DCTDecode filter decodes grayscale or colour image data that has been encoded in the JPEG baseline format
	* \details See Adobe Technical Note #5116 for additional information about the use of JPEG "markers".<br>
	* For more information please visit [section 7.4.8 - DCTDecode Filter](PDF32000_2008.pdf#G6.1877438)
	*/

	/**
	* \class ASCII85DecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup group_files
	* \brief The ASCII85Decode filter decodes data that has been encoded in [ASCII base-85 encoding](https://en.wikipedia.org/wiki/Ascii85) and produces binary data
	* \details For more information please visit [section 7.4.3 - ASCII85Decode Filter>](PDF32000_2008.pdf#G6.1940130)
	*/

	/**
	* \class ASCIIHexDecodeFilterHandle
	* \extends FilterBaseHandle
	* \ingroup group_files
	* \brief The ASCIIHexDecode filter decodes data that has been encoded in ASCII hexadecimal form
	* \details ASCII hexadecimal encoding and \ref ASCII85DecodeFilterHandle "ASCII base-85 encoding" convert binary data,
	* such as image data or previously compressed data, to 7-bit ASCII characters.<br>
	* For more information please visit [section 7.4.2 - ASCIIHexDecode Filter](PDF32000_2008.pdf#G6.1639097)
	*/

	/**
	* \memberof FilterBaseHandle
	* @{
	*/

	/**
	* \brief Encodes source \p data and returns encoded \p result data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_Encode(FilterBaseHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \brief Encodes source \p data with specified \p params and returns encoded \p result data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_EncodeParams(FilterBaseHandle* handle, BufferHandle* data, DictionaryObjectHandle* parameters, BufferHandle** result);

	/**
	* \brief Decodes source \p data and returns decoded \p result data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_Decode(FilterBaseHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FilterBase_Release(FilterBaseHandle* handle);

	/** @} */

	/**
	* \memberof FlateDecodeFilterHandle
	* @{
	*/

	/**
	* \brief Creates a new filter instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Create(FlateDecodeFilterHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_Encode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Encode(FlateDecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_EncodeParams
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_EncodeParams(FlateDecodeFilterHandle* handle, BufferHandle* data, DictionaryObjectHandle* parameters, BufferHandle** result);

	/**
	* \copydoc FilterBase_Decode
	* \see \ref FilterBase_Decode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Decode(FlateDecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION FlateDecodeFilter_Release(FlateDecodeFilterHandle* handle);

	/** @} */

	/**
	* \memberof DCTDecodeFilterHandle
	* @{
	*/

	/**
	* \brief Creates a new filter instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Create(DCTDecodeFilterHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_Encode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Encode(DCTDecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_EncodeParams
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_EncodeParams(DCTDecodeFilterHandle* handle, BufferHandle* data, DictionaryObjectHandle* parameters, BufferHandle** result);

	/**
	* \copydoc FilterBase_Decode
	* \see \ref FilterBase_Decode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Decode(DCTDecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DCTDecodeFilter_Release(DCTDecodeFilterHandle* handle);

	/** @} */

	/**
	* \memberof ASCII85DecodeFilterHandle
	* @{
	*/

	/**
	* \brief Creates a new filter instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Create(ASCII85DecodeFilterHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_Encode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Encode(ASCII85DecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_EncodeParams
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_EncodeParams(ASCII85DecodeFilterHandle* handle, BufferHandle* data, DictionaryObjectHandle* parameters, BufferHandle** result);

	/**
	* \copydoc FilterBase_Decode
	* \see \ref FilterBase_Decode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Decode(ASCII85DecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCII85DecodeFilter_Release(ASCII85DecodeFilterHandle* handle);

	/** @} */

	/**
	* \memberof ASCIIHexDecodeFilterHandle
	* @{
	*/

	/**
	* \brief Creates a new filter instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Create(ASCIIHexDecodeFilterHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_Encode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Encode(ASCIIHexDecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Encode
	* \see \ref FilterBase_EncodeParams
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_EncodeParams(ASCIIHexDecodeFilterHandle* handle, BufferHandle* data, DictionaryObjectHandle* parameters, BufferHandle** result);

	/**
	* \copydoc FilterBase_Decode
	* \see \ref FilterBase_Decode
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Decode(ASCIIHexDecodeFilterHandle* handle, BufferHandle* data, BufferHandle** result);

	/**
	* \copydoc FilterBase_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ASCIIHexDecodeFilter_Release(ASCIIHexDecodeFilterHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILTER_H */
