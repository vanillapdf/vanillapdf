#ifndef _C_BOOLEAN_OBJECT_H
#define _C_BOOLEAN_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_boolean_object.h
    * \brief This file contains class definitions for \ref BooleanObjectHandle
    */

    /**
    * \class BooleanObjectHandle
    * \extends ObjectHandle
    * \ingroup group_objects
    * \brief Boolean objects represent the logical values of true and false.
    */

    /**
    * \memberof BooleanObjectHandle
    * @{
    */

    /**
    * \brief Creates a new instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_Create(BooleanObjectHandle** result);

    /**
    * \brief Return current boolean value
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_GetValue(BooleanObjectHandle* handle, boolean_type* result);

    /**
    * \brief Set new boolean value
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_SetValue(BooleanObjectHandle* handle, boolean_type value);

    /**
    * \brief Reinterpret current object as \ref ObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_ToObject(BooleanObjectHandle* handle, ObjectHandle** result);

    /**
    * \brief Convert \ref ObjectHandle to \ref BooleanObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_FromObject(ObjectHandle* handle, BooleanObjectHandle** result);

    /**
    * \copydoc Object_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BOOLEAN_OBJECT_H */
