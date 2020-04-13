#ifndef _C_INDIRECT_OBJECT_REFERENCE_H
#define _C_INDIRECT_OBJECT_REFERENCE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_indirect_object_reference.h
	* \brief This file contains class definitions for \ref IndirectObjectReferenceHandle
	*/

	/**
	* \class IndirectObjectReferenceHandle
	* \extends ObjectHandle
	* \ingroup group_objects
	* \brief Represents reference to other object
	*/

	/**
	* \memberof IndirectObjectReferenceHandle
	* @{
	*/

	/**
	* \brief Creates a new IndirectObjectReference instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_Create(IndirectObjectReferenceHandle** result);

	/**
	* \brief Return object number of referenced object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_GetReferencedObjectNumber(IndirectObjectReferenceHandle* handle, biguint_type* result);

	/**
	* \brief Return generation number of referenced object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_GetReferencedGenerationNumber(IndirectObjectReferenceHandle* handle, ushort_type* result);

	/**
	* \brief Return object handle to referenced object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_GetReferencedObject(IndirectObjectReferenceHandle* handle, ObjectHandle** result);

	/**
	* \brief Sets a new referenced object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_SetReferencedObject(IndirectObjectReferenceHandle* handle, ObjectHandle* value);

	/**
	* \brief Reinterpret current object as \ref ObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_ToObject(IndirectObjectReferenceHandle* handle, ObjectHandle** result);

	/**
	* \brief Convert \ref ObjectHandle to \ref IndirectObjectReferenceHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_FromObject(ObjectHandle* handle, IndirectObjectReferenceHandle** result);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_Release(IndirectObjectReferenceHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_OBJECT_REFERENCE_H */
