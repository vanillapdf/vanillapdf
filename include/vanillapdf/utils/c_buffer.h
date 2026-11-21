#ifndef _C_BUFFER_H
#define _C_BUFFER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_buffer.h
    * \brief This file contains class definitions for \ref BufferHandle
    */

    /**
    * \class BufferHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief Represents memory stored data
    *
    * A buffer is a container of bytes and does not interpret them.
    * The function that returns a buffer specifies what the bytes mean.
    */

    /**
    * \memberof BufferHandle
    * @{
    */

    /**
    * \brief Creates a new buffer instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Create(BufferHandle** result);

    /**
    * \brief Creates a new buffer instance with specified data
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_CreateFromData(string_type data, size_type size, BufferHandle** result);

    /**
    * \brief Get data content from buffer
    *
    * The size is the number of bytes in the buffer.
    * By default, buffer data does not include a null terminator.
    * Callers that treat the data as a string must not assume null termination
    * unless the function that returned the buffer specifies it.
    * \trivial
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_GetData(const BufferHandle* handle, string_type* data, size_type* size);

    /**
    * \brief Set new data content
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_SetData(BufferHandle* handle, string_type data, size_type size);

    /**
    * \brief Copy buffer data to the memory provided by the caller
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_CopyTo(BufferHandle* handle, byte_array_type data, size_type size);

    /**
    * \brief Converts current buffer state to input stream
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_ToInputStream(const BufferHandle* handle, InputStreamHandle** result);

    /**
    * \brief Compares two buffers by their value
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Equals(const BufferHandle* handle, const BufferHandle* other, boolean_type* result);

    /**
    * \brief Get object hash code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Hash(const BufferHandle* handle, size_type* result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    * \trivial
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_ToUnknown(BufferHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref BufferHandle
    * \trivial
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_FromUnknown(IUnknownHandle* handle, BufferHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BUFFER_H */
