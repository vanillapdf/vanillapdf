#ifndef _C_INDIRECT_OBJECT_REFERENCE_H
#define _C_INDIRECT_OBJECT_REFERENCE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_indirect_object_reference.h
	* This file contains class definitions for IndirectObjectReferenceHandle
	*/

	/**
	* \class IndirectObjectReferenceHandle
	* \extends ObjectHandle
	* \ingroup Objects
	* \brief Represents reference to other object
	*/

	/**
	* \memberof IndirectObjectReferenceHandle
	* @{
	*/

	/**
	* \brief Return object number of referenced object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObjectNumber(IndirectObjectReferenceHandle handle, biguint_type* result);

	/**
	* \brief Return generation number of referenced object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedGenerationNumber(IndirectObjectReferenceHandle handle, ushort_type* result);

	/**
	* \brief Return object handle to referenced object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle handle, ObjectHandle* result);

	/**
	* \copydoc Object_Release()
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IndirectReference_Release(IndirectObjectReferenceHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_OBJECT_REFERENCE_H */
