#ifndef _C_INTEGER_OBJECT_H
#define _C_INTEGER_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_integer_object.h
    * \brief This file contains class definitions for \ref IntegerObjectHandle
    */

    /**
    * \class IntegerObjectHandle
    * \extends ObjectHandle
    * \ingroup group_objects
    * \brief Integer objects represent mathematical integers
    */

    /**
    * \memberof IntegerObjectHandle
    * @{
    */

    /**
    * \brief Creates a new IntegerObject instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_Create(IntegerObjectHandle** result);

    /**
    * \brief Return objects contained integer value
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_GetIntegerValue(IntegerObjectHandle* handle, bigint_type* result);

    /**
    * \brief Return objects contained unsigned integer value
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_GetUnsignedIntegerValue(IntegerObjectHandle* handle, biguint_type* result);

    /**
    * \brief Set objects new integer value
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_SetIntegerValue(IntegerObjectHandle* handle, bigint_type value);

    /**
    * \brief Set objects new unsigned integer value
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_SetUnsignedIntegerValue(IntegerObjectHandle* handle, biguint_type value);

    /**
    * \brief Reinterpret current object as \ref ObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_ToObject(IntegerObjectHandle* handle, ObjectHandle** result);

    /**
    * \brief Convert \ref ObjectHandle to \ref IntegerObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_FromObject(ObjectHandle* handle, IntegerObjectHandle** result);

    /**
    * \copydoc Object_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INTEGER_OBJECT_H */
