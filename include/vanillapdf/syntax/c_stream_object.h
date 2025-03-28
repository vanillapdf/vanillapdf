#ifndef _C_STREAM_OBJECT_H
#define _C_STREAM_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_stream_object.h
    * \brief This file contains class definitions for \ref StreamObjectHandle
    */

    /**
    * \class StreamObjectHandle
    * \extends ObjectHandle
    * \ingroup group_objects
    * \brief Stream object represents compressed data inside document
    */

    /**
    * \memberof StreamObjectHandle
    * @{
    */

    /**
    * \brief Creates a new StreamObject instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_Create(StreamObjectHandle** result);

    /**
    * \brief Return streams header dictionary
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_GetHeader(StreamObjectHandle* handle, DictionaryObjectHandle** result);

    /**
    * \brief Set new streams header dictionary
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_SetHeader(StreamObjectHandle* handle, DictionaryObjectHandle* value);

    /**
    * \brief Get uncompressed stream body
    *
    * This function is useful when the library cannot decompress or decrypt the stream data.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_GetBodyRaw(StreamObjectHandle* handle, BufferHandle** result);

    /**
    * \brief Get decompressed stream content
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_GetBody(StreamObjectHandle* handle, BufferHandle** result);

    /**
    * \brief Set new stream content
    *
    * This sets the decompressed content. Compression is done when the file is saved.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_SetBody(StreamObjectHandle* handle, BufferHandle* value);

    /**
    * \brief Reinterpret current object as \ref ObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_ToObject(StreamObjectHandle* handle, ObjectHandle** result);

    /**
    * \brief Convert \ref ObjectHandle to \ref StreamObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_FromObject(ObjectHandle* handle, StreamObjectHandle** result);

    /**
    * \copydoc Object_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_Release(StreamObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_STREAM_OBJECT_H */
