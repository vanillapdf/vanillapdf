#ifndef _C_ARRAY_OBJECT_H
#define _C_ARRAY_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_array_object.h
    * \brief This file contains class definitions for \ref ArrayObjectHandle
    */

    /**
    * \class ArrayObjectHandle
    * \extends ObjectHandle
    * \ingroup group_objects
    * \brief An array object is a one-dimensional collection of objects arranged sequentially
    */

    /**
    * \memberof ArrayObjectHandle
    * @{
    */

    /**
    * \brief Creates a new ArrayObject instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Create(ArrayObjectHandle** result);

    /**
    * \brief Return size of an array
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_GetSize(ArrayObjectHandle* handle, size_type* result);

    /**
    * \brief Get element at location \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_GetValue(ArrayObjectHandle* handle, size_type at, ObjectHandle** result);

    /**
    * \brief Set element at location \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_SetValue(ArrayObjectHandle* handle, size_type at, ObjectHandle* value);

    /**
    * \brief Insert new element at the end of the array
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayObjectHandle* handle, ObjectHandle* value);

    /**
    * \brief Insert new element at location \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayObjectHandle* handle, size_type at, ObjectHandle* value);

    /**
    * \brief Remove element from location \p at
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayObjectHandle* handle, size_type at);

    /**
    * \brief Clear all items from the collection
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Clear(ArrayObjectHandle* handle);

    /**
    * \brief Reinterpret current object as \ref ObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_ToObject(ArrayObjectHandle* handle, ObjectHandle** result);

    /**
    * \brief Convert \ref ObjectHandle to \ref ArrayObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_FromObject(ObjectHandle* handle, ArrayObjectHandle** result);

    /**
    * \copydoc Object_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ARRAY_OBJECT_H */
