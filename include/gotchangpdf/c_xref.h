#ifndef _C_XREF_H
#define _C_XREF_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum {
		XrefEntryNull,
		XrefEntryFree,
		XrefEntryUsed,
		XrefEntryCompressed
	} XrefEntryType, *PXrefEntryType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Type(XrefEntryHandle handle, PXrefEntryType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ObjectNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_GenerationNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToFreeEntry(XrefEntryHandle handle, PXrefFreeEntryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToUsedEntry(XrefEntryHandle handle, PXrefUsedEntryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToCompressedEntry(XrefEntryHandle handle, PXrefCompressedEntryHandle result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_ObjectNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_GenerationNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_InUse(XrefFreeEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_NextFreeObjectNumber(XrefFreeEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_Release(XrefFreeEntryHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_ObjectNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_GenerationNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Offset(XrefUsedEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_InUse(XrefUsedEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Reference(XrefUsedEntryHandle handle, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Release(XrefUsedEntryHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GenerationNumber(XrefEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_InUse(XrefCompressedEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Reference(XrefCompressedEntryHandle handle, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Index(XrefCompressedEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectStreamNumber(XrefCompressedEntryHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Release(XrefCompressedEntryHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_TrailerDictionary(XrefHandle handle, PDictionaryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_LastXrefOffset(XrefHandle handle, out_offset_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Size(XrefHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_At(XrefHandle handle, integer_type at, PXrefSubsectionHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Index(XrefSubsectionHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Size(XrefSubsectionHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_At(XrefSubsectionHandle handle, integer_type at, PXrefEntryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Release(XrefSubsectionHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_GetValue(XrefChainIteratorHandle handle, PXrefHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_IsValid(XrefChainIteratorHandle handle, XrefChainHandle chain, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Next(XrefChainIteratorHandle handle);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Release(XrefChainIteratorHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Iterator(XrefChainHandle handle, PXrefChainIteratorHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Release(XrefChainHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_XREF_H */
