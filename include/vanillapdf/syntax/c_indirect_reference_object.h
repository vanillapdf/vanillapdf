#ifndef _C_INDIRECT_REFERENCE_OBJECT_H
#define _C_INDIRECT_REFERENCE_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_indirect_reference_object.h
    * \brief This file contains class definitions for \ref IndirectReferenceObjectHandle
    */

    /**
    * \class IndirectReferenceObjectHandle
    * \extends ObjectHandle
    * \ingroup group_objects
    * \brief Represents reference to another object
    */

    /**
    * \memberof IndirectReferenceObjectHandle
    * @{
    */

    /**
    * \brief Creates a new IndirectReferenceObject instance
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_Create(IndirectReferenceObjectHandle** result);

    /**
    * \brief Return object number of referenced object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_GetReferencedObjectNumber(IndirectReferenceObjectHandle* handle, biguint_type* result);

    /**
    * \brief Return generation number of referenced object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_GetReferencedGenerationNumber(IndirectReferenceObjectHandle* handle, ushort_type* result);

    /**
    * \brief Return object handle to referenced object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_GetReferencedObject(IndirectReferenceObjectHandle* handle, ObjectHandle** result);

    /**
    * \brief Sets a new referenced object
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_SetReferencedObject(IndirectReferenceObjectHandle* handle, ObjectHandle* value);

    /**
    * \brief Reinterpret current object as \ref ObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_ToObject(IndirectReferenceObjectHandle* handle, ObjectHandle** result);

    /**
    * \brief Convert \ref ObjectHandle to \ref IndirectReferenceObjectHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_FromObject(ObjectHandle* handle, IndirectReferenceObjectHandle** result);

    /**
    * \copydoc Object_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_Release(IndirectReferenceObjectHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_REFERENCE_OBJECT_H */
