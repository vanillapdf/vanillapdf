#ifndef _C_INDIRECT_OBJECT_REFERENCE_H
#define _C_INDIRECT_OBJECT_REFERENCE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_indirect_object_reference.h
	* This file contains contains class definitions for IndirectObjectReferenceHandle
	*/

	/**
	* \brief Return object number of referenced object
	* \memberof IndirectObjectReferenceHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObjectNumber(IndirectObjectReferenceHandle handle, out_biguint_type result);

	/**
	* \brief Return generation number of referenced object
	* \memberof IndirectObjectReferenceHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedGenerationNumber(IndirectObjectReferenceHandle handle, out_ushort_type result);

	/**
	* \brief Return object handle to referenced object
	* \memberof IndirectObjectReferenceHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle handle, PObjectHandle result);

	/**
	* \memberof IndirectObjectReferenceHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_Release(IndirectObjectReferenceHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_OBJECT_REFERENCE_H */
