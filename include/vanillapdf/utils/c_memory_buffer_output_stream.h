#ifndef _C_MEMORY_BUFFER_OUTPUT_STREAM_H
#define _C_MEMORY_BUFFER_OUTPUT_STREAM_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_memory_buffer_output_stream.h
    * \brief This file contains class definitions for \ref MemoryBufferOutputStreamHandle
    */

    /**
    * \class MemoryBufferOutputStreamHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief An output stream that writes data to an internal memory buffer.
    *
    * This stream implementation uses `fmt::memory_buffer` internally to accumulate
    * character or binary data. It is typically used when dynamically collecting
    * output into a contiguous memory block, for later retrieval or further processing.
    *
    * The data written to this stream is stored in a resizable buffer, which can be
    * accessed or converted to a `BufferHandle` after writing is complete.
    *
    * \note This stream is not thread-safe.
    */

    /**
    * \memberof MemoryBufferOutputStreamHandle
    * @{
    */

    /**
    * \brief Creates a new file at \p filename location and opens it for writing
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_Create(MemoryBufferOutputStreamHandle** result);

    /**
    * \brief Gets current offset in the output stream
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_GetOutputPosition(MemoryBufferOutputStreamHandle* handle, offset_type* result);

    /**
    * \brief Sets current offset in the output stream
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_SetOutputPosition(MemoryBufferOutputStreamHandle* handle, offset_type value);

    /**
    * \brief Appends null terminated string data to current output stream instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_WriteString(MemoryBufferOutputStreamHandle* handle, string_type data);

    /**
    * \brief Appends buffer data to current output stream instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_WriteBuffer(MemoryBufferOutputStreamHandle* handle, BufferHandle* data);

    /**
    * \brief Appends buffer data to current output stream instance up to length specified
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_WriteBufferRange(MemoryBufferOutputStreamHandle* handle, BufferHandle* data, offset_type length);

    /**
    * \brief Flushes all pending data from the stream to it's destination
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_Flush(MemoryBufferOutputStreamHandle* handle);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_ToUnknown(MemoryBufferOutputStreamHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref OutputStreamHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_FromUnknown(IUnknownHandle* handle, MemoryBufferOutputStreamHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_Release(MemoryBufferOutputStreamHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_MEMORY_BUFFER_OUTPUT_STREAM_H */
