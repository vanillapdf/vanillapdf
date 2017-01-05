#ifndef _C_XREF_H
#define _C_XREF_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_xref.h
	* This file contains class definitions for cross-reference tables, chained tables and entries.
	*/

	/**
	* \brief Required for conversion to derived types.
	*/
	typedef enum
	{
		/**
		* \brief This type is not actually used.
		*
		* It represents undefined type only to distinguish from other types.
		* When an entry is created, this is the default value to mark it uninitialized entry.
		*
		* No function should return this value.
		* If it does, it is a bug.
		*
		* The reason it exists is that entry needs to have a default constructor.
		* This is only entry type that have default constructor.
		*/
		XrefEntryType_Null = 0,

		/**
		* \copybrief XrefFreeEntryHandle
		* \see XrefFreeEntryHandle
		*/
		XrefEntryType_Free,

		/**
		* \copybrief XrefUsedEntryHandle
		* \see XrefUsedEntryHandle
		*/
		XrefEntryType_Used,

		/**
		* \copybrief XrefCompressedEntryHandle
		* \see XrefCompressedEntryHandle
		*/
		XrefEntryType_Compressed
	} XrefEntryType;

	/**
	* \brief Pointer to XrefEntryType
	*/
	typedef XrefEntryType *PXrefEntryType;

	/**
	* \memberof XrefHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_TrailerDictionary(XrefHandle handle, PDictionaryObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_LastXrefOffset(XrefHandle handle, out_offset_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Iterator(XrefHandle handle, PXrefIteratorHandle result);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle handle);

	/** @} */

	/**
	* \memberof XrefIteratorHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_GetValue(XrefIteratorHandle handle, PXrefEntryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_IsValid(XrefIteratorHandle handle, XrefHandle xref, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_Next(XrefIteratorHandle handle);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_Release(XrefIteratorHandle handle);

	/** @} */

	/**
	* \memberof XrefChainHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Iterator(XrefChainHandle handle, PXrefChainIteratorHandle result);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Release(XrefChainHandle handle);

	/** @} */

	/**
	* \memberof XrefChainIteratorHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_GetValue(XrefChainIteratorHandle handle, PXrefHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_IsValid(XrefChainIteratorHandle handle, XrefChainHandle chain, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Next(XrefChainIteratorHandle handle);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Release(XrefChainIteratorHandle handle);

	/** @} */

	/**
	* \memberof XrefEntryHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Type(XrefEntryHandle handle, PXrefEntryType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ObjectNumber(XrefEntryHandle handle, out_biguint_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_GenerationNumber(XrefEntryHandle handle, out_ushort_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle handle, out_boolean_type result);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToFreeEntry(XrefEntryHandle handle, PXrefFreeEntryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToUsedEntry(XrefEntryHandle handle, PXrefUsedEntryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToCompressedEntry(XrefEntryHandle handle, PXrefCompressedEntryHandle result);

	/** @} */

	/**
	* \memberof XrefFreeEntryHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_ObjectNumber(XrefEntryHandle handle, out_biguint_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_GenerationNumber(XrefEntryHandle handle, out_ushort_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_InUse(XrefFreeEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_NextFreeObjectNumber(XrefFreeEntryHandle handle, out_biguint_type result);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_Release(XrefFreeEntryHandle handle);

	/** @} */

	/**
	* \memberof XrefUsedEntryHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_ObjectNumber(XrefEntryHandle handle, out_biguint_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_GenerationNumber(XrefEntryHandle handle, out_ushort_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Offset(XrefUsedEntryHandle handle, out_offset_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_InUse(XrefUsedEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Reference(XrefUsedEntryHandle handle, PObjectHandle result);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Release(XrefUsedEntryHandle handle);

	/** @} */

	/**
	* \memberof XrefCompressedEntryHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectNumber(XrefEntryHandle handle, out_biguint_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GenerationNumber(XrefEntryHandle handle, out_ushort_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_InUse(XrefCompressedEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Reference(XrefCompressedEntryHandle handle, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Index(XrefCompressedEntryHandle handle, out_uinteger_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectStreamNumber(XrefCompressedEntryHandle handle, out_biguint_type result);

	/**
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Release(XrefCompressedEntryHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_XREF_H */
