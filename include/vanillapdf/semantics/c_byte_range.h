#ifndef _C_BYTE_RANGE_H
#define _C_BYTE_RANGE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_byte_range.h
    * \brief This file contains class definitions for \ref ByteRangeHandle and \ref ByteRangeCollectionHandle
    */

    /**
    * \class ByteRangeCollectionHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief Represents a collection of \ref ByteRangeHandle
    *
    * ByteRangeCollection is used to specify exact byte ranges for digest calculation
    * in digital signatures. Multiple discontiguous byte ranges are used to describe
    * a digest that does not include the signature value itself.
    */

    /**
    * \class ByteRangeHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief Represents starting byte offset and length
    *
    * ByteRange represents a single contiguous byte range in a PDF file,
    * defined by a starting offset and a length.
    */

    /**
    * \memberof ByteRangeCollectionHandle
    * @{
    */

    /**
    * \brief Create an empty ByteRangeCollection
    * \param result Output ByteRangeCollection handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Create(ByteRangeCollectionHandle** result);

    /**
    * \brief Append a ByteRange to the end of the collection
    * \param handle ByteRangeCollection handle
    * \param byte_range ByteRange to append
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Append(ByteRangeCollectionHandle* handle, ByteRangeHandle* byte_range);

    /**
    * \brief Insert a ByteRange at a specific position in the collection
    * \param handle ByteRangeCollection handle
    * \param at Position to insert at
    * \param byte_range ByteRange to insert
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Insert(ByteRangeCollectionHandle* handle, size_type at, ByteRangeHandle* byte_range);

    /**
    * \brief Remove a ByteRange at a specific position
    * \param handle ByteRangeCollection handle
    * \param at Position to remove from
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Remove(ByteRangeCollectionHandle* handle, size_type at);

    /**
    * \brief Remove all ByteRanges from the collection
    * \param handle ByteRangeCollection handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Clear(ByteRangeCollectionHandle* handle);

    /**
    * \brief Get size of byte range collection
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_GetSize(ByteRangeCollectionHandle* handle, size_type* result);

    /**
    * \brief
    * Get single byte range from array at specific position
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_GetValue(ByteRangeCollectionHandle* handle, size_type at, ByteRangeHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Release(ByteRangeCollectionHandle* handle);

    /** @} */

    /**
    * \memberof ByteRangeHandle
    * @{
    */

    /**
    * \brief Create an empty ByteRange
    * \param result Output ByteRange handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_Create(ByteRangeHandle** result);

    /**
    * \brief Create a ByteRange with offset and length values
    * \param offset Starting byte offset
    * \param length Length in bytes
    * \param result Output ByteRange handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_CreateFromData(IntegerObjectHandle* offset, IntegerObjectHandle* length, ByteRangeHandle** result);

    /**
    * \brief Get starting offset of byte range
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetOffset(ByteRangeHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief Get length of byte range
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetLength(ByteRangeHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief Set starting offset of byte range
    * \param handle ByteRange handle
    * \param offset New starting byte offset
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_SetOffset(ByteRangeHandle* handle, IntegerObjectHandle* offset);

    /**
    * \brief Set length of byte range
    * \param handle ByteRange handle
    * \param length New length in bytes
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_SetLength(ByteRangeHandle* handle, IntegerObjectHandle* length);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_Release(ByteRangeHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BYTE_RANGE_H */
