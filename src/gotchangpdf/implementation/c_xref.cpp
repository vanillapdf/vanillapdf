#include "precompiled.h"

#include "syntax/files/xref.h"
#include "syntax/files/xref_chain.h"

#include "gotchangpdf/c_xref.h"
#include "gotchangpdf/c_values.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_TrailerDictionary(XrefHandle handle, DictionaryObjectHandle* result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto dict = xref->GetTrailerDictionary();
		auto ptr = dict.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_LastXrefOffset(XrefHandle handle, offset_type* result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = xref->GetLastXrefOffset();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Iterator(XrefHandle handle, XrefIteratorHandle* result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto begin = xref->Begin();
		auto ptr = begin.AddRefGet();
		*result = reinterpret_cast<XrefIteratorHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_GetValue(XrefIteratorHandle handle, XrefEntryHandle* result)
{
	XrefBase::Iterator* iterator = reinterpret_cast<XrefBase::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto entry = iterator->Value();
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefEntryHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_IsValid(XrefIteratorHandle handle, XrefHandle xref_handle, boolean_type* result)
{
	XrefBase::Iterator* iterator = reinterpret_cast<XrefBase::Iterator*>(handle);
	XrefBase* xref = reinterpret_cast<XrefBase*>(xref_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		if (*xref->End() == *iterator)
			*result = GOTCHANG_PDF_RV_FALSE;
		else
			*result = GOTCHANG_PDF_RV_TRUE;

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_Next(XrefIteratorHandle handle)
{
	XrefBase::Iterator* iterator = reinterpret_cast<XrefBase::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try {
		++(*iterator);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefIterator_Release(XrefIteratorHandle handle)
{
	return ObjectRelease<XrefBase::Iterator, XrefIteratorHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle handle)
{
	return ObjectRelease<XrefBase, XrefHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	return ObjectRelease<XrefEntryBase, XrefEntryHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle handle, boolean_type* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	if (entry->InUse())
		*result = GOTCHANG_PDF_RV_TRUE;
	else
		*result = GOTCHANG_PDF_RV_FALSE;

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Type(XrefEntryHandle handle, XrefEntryType* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	// I do not use Null entry intentionally - it is only for default construction
	// It might be something going on
	assert(entry->GetUsage() != XrefEntryBase::Usage::Null);

	switch (entry->GetUsage()) {
	case XrefEntryBase::Usage::Free:
		*result = XrefEntryType_Free; break;
	case XrefEntryBase::Usage::Used:
		*result = XrefEntryType_Used; break;
	case XrefEntryBase::Usage::Compressed:
		*result = XrefEntryType_Compressed; break;
	case XrefEntryBase::Usage::Null:
		*result = XrefEntryType_Null; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_InUse(XrefFreeEntryHandle handle, boolean_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->InUse();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_NextFreeObjectNumber(XrefFreeEntryHandle handle, biguint_type* result)
{
	XrefFreeEntry* entry = reinterpret_cast<XrefFreeEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetNextFreeObjectNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefFreeEntry_Release(XrefFreeEntryHandle handle)
{
	return ObjectRelease<XrefFreeEntry, XrefFreeEntryHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Offset(XrefUsedEntryHandle handle, offset_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetOffset();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_InUse(XrefUsedEntryHandle handle, boolean_type* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->InUse();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Release(XrefUsedEntryHandle handle)
{
	return ObjectRelease<XrefUsedEntry, XrefUsedEntryHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectNumber(XrefEntryHandle handle, biguint_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_GenerationNumber(XrefEntryHandle handle, ushort_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetGenerationNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_InUse(XrefCompressedEntryHandle handle, boolean_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->InUse();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Index(XrefCompressedEntryHandle handle, size_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetIndex();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_ObjectStreamNumber(XrefCompressedEntryHandle handle, biguint_type* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = entry->GetObjectStreamNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Release(XrefCompressedEntryHandle handle)
{
	return ObjectRelease<XrefCompressedEntry, XrefCompressedEntryHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToFreeEntry(XrefEntryHandle handle, XrefFreeEntryHandle* result)
{
	return SafeObjectConvert<XrefEntryBase, XrefFreeEntry, XrefEntryHandle, XrefFreeEntryHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToUsedEntry(XrefEntryHandle handle, XrefUsedEntryHandle* result)
{
	return SafeObjectConvert<XrefEntryBase, XrefUsedEntry, XrefEntryHandle, XrefUsedEntryHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToCompressedEntry(XrefEntryHandle handle, XrefCompressedEntryHandle* result)
{
	return SafeObjectConvert<XrefEntryBase, XrefCompressedEntry, XrefEntryHandle, XrefCompressedEntryHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Reference(XrefUsedEntryHandle handle, ObjectHandle* result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = entry->GetReference();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Reference(XrefCompressedEntryHandle handle, ObjectHandle* result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = entry->GetReference();

		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_GetValue(XrefChainIteratorHandle handle, XrefHandle* result)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto entry = iterator->Value();
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_IsValid(XrefChainIteratorHandle handle, XrefChainHandle chain_handle, boolean_type* result)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	XrefChain* chain = reinterpret_cast<XrefChain*>(chain_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(chain);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		if (*chain->End() == *iterator)
			*result = GOTCHANG_PDF_RV_FALSE;
		else
			*result = GOTCHANG_PDF_RV_TRUE;

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Next(XrefChainIteratorHandle handle)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try {
		++(*iterator);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Release(XrefChainIteratorHandle handle)
{
	return ObjectRelease<XrefChain::Iterator, XrefChainIteratorHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Iterator(XrefChainHandle handle, XrefChainIteratorHandle* result)
{
	XrefChain* chain = reinterpret_cast<XrefChain*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(chain);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto begin = chain->Begin();
		auto ptr = begin.AddRefGet();
		*result = reinterpret_cast<XrefChainIteratorHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Release(XrefChainHandle handle)
{
	return ObjectRelease<XrefChain, XrefChainHandle>(handle);
}
