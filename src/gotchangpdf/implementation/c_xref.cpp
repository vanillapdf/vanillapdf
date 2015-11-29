#include "precompiled.h"
#include "xref.h"
#include "xref_chain.h"

#include "file.h"
#include "object_visitors.h"

#include "c_xref.h"
#include "c_values.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_TrailerDictionary(XrefHandle handle, PDictionaryHandle result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	//LOG_SCOPE(chain->GetFile()->GetFilename());

	try
	{
		auto dict = xref->GetTrailerDictionary();
		auto ptr = dict.AddRefGet();
		*result = reinterpret_cast<DictionaryHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_LastXrefOffset(XrefHandle handle, out_offset_type result)
{
	XrefBase* xref = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(xref);
	//LOG_SCOPE(xref->GetParent()->GetFile()->GetFilename());

	*result = xref->GetLastXrefOffset();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Size(XrefHandle handle, out_integer_type result)
{
	XrefBase* table = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(table);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(table->GetFile()->GetFilename());

	*result = table->Size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_At(XrefHandle handle, integer_type at, PXrefSubsectionHandle result)
{
	XrefBase* table = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(table);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(table->GetFile()->GetFilename());

	try
	{
		auto entry = table->At(at);
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefSubsectionHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle handle)
{
	XrefBase* table = reinterpret_cast<XrefBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(table);
	LOG_SCOPE(table->GetFile()->GetFilename());

	table->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	entry->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle handle, out_boolean_type result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	if (entry->InUse())
		*result = GOTCHANG_PDF_RV_TRUE;
	else
		*result = GOTCHANG_PDF_RV_FALSE;

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Type(XrefEntryHandle handle, PXrefEntryType result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	// I do not use Null entry intentionally - it is only for default construction
	// It might be something going on
	assert(entry->GetUsage() != XrefEntryBase::Usage::Null);

	switch (entry->GetUsage()) {
	case XrefEntryBase::Usage::Free:
		*result = XrefEntryFree; break;
	case XrefEntryBase::Usage::Used:
		*result = XrefEntryUsed; break;
	case XrefEntryBase::Usage::Compressed:
		*result = XrefEntryCompressed; break;
	case XrefEntryBase::Usage::Null:
		*result = XrefEntryNull; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToFreeEntry(XrefEntryHandle handle, PXrefFreeEntryHandle result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	try {
		XrefFreeEntry* converted = dynamic_cast<XrefFreeEntry*>(entry);
		*result = reinterpret_cast<XrefFreeEntryHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToUsedEntry(XrefEntryHandle handle, PXrefUsedEntryHandle result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	try {
		XrefUsedEntry* converted = dynamic_cast<XrefUsedEntry*>(entry);
		*result = reinterpret_cast<XrefUsedEntryHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToCompressedEntry(XrefEntryHandle handle, PXrefCompressedEntryHandle result)
{
	XrefEntryBase* entry = reinterpret_cast<XrefEntryBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	try {
		XrefCompressedEntry* converted = dynamic_cast<XrefCompressedEntry*>(entry);
		*result = reinterpret_cast<XrefCompressedEntryHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefUsedEntry_Reference(XrefUsedEntryHandle handle, PObjectHandle result)
{
	XrefUsedEntry* entry = reinterpret_cast<XrefUsedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	try {
		auto direct = entry->GetReference();

		ObjectBaseAddRefVisitor visitor;
		auto base = direct.apply_visitor(visitor);
		*result = reinterpret_cast<ObjectHandle>(base);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefCompressedEntry_Reference(XrefCompressedEntryHandle handle, PObjectHandle result)
{
	XrefCompressedEntry* entry = reinterpret_cast<XrefCompressedEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	try {
		auto direct = entry->GetReference();

		ObjectBaseAddRefVisitor visitor;
		auto base = direct.apply_visitor(visitor);
		*result = reinterpret_cast<ObjectHandle>(base);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Index(XrefSubsectionHandle handle, out_integer_type result)
{
	XrefSubsection* section = reinterpret_cast<XrefSubsection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(section);
	//LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	*result = section->Index();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Size(XrefSubsectionHandle handle, out_integer_type result)
{
	XrefSubsection* section = reinterpret_cast<XrefSubsection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(section);
	//LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	*result = section->Size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_At(XrefSubsectionHandle handle, integer_type at, PXrefEntryHandle result)
{
	XrefSubsection* section = reinterpret_cast<XrefSubsection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(section);
	//LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	try {
		auto entry = section->At(at);
		XrefEntryBaseVisitor visitor;
		auto ptr = entry.apply_visitor(visitor);
		ptr->AddRef();
		*result = reinterpret_cast<XrefEntryHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Release(XrefSubsectionHandle handle)
{
	XrefSubsection* section = reinterpret_cast<XrefSubsection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(section);
	//LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	section->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_GetValue(XrefChainIteratorHandle handle, PXrefHandle result)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	//LOG_SCOPE(chain->GetParent()->GetFile()->GetFilename());

	try {
		auto entry = iterator->Value();
		XrefBaseVisitor visitor;
		auto ptr = entry.apply_visitor(visitor);
		ptr->AddRef();
		*result = reinterpret_cast<XrefHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_IsValid(XrefChainIteratorHandle handle, XrefChainHandle chain_handle, out_boolean_type result)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	XrefChain* chain = reinterpret_cast<XrefChain*>(chain_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(chain);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(dictionary->GetFile()->GetFilename());

	try {
		if (*chain->End() == *iterator)
			*result = GOTCHANG_PDF_RV_FALSE;
		else
			*result = GOTCHANG_PDF_RV_TRUE;

		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Next(XrefChainIteratorHandle handle)
{
	XrefChain::Iterator* iterator = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try {
		++(*iterator);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChainIterator_Release(XrefChainIteratorHandle handle)
{
	XrefChain::Iterator* it = reinterpret_cast<XrefChain::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(it);
	//LOG_SCOPE(chain->GetFile()->GetFilename());

	it->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Iterator(XrefChainHandle handle, PXrefChainIteratorHandle result)
{
	XrefChain* chain = reinterpret_cast<XrefChain*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(chain);
	//LOG_SCOPE(chain->GetFile()->GetFilename());

	auto begin = chain->Begin();
	auto ptr = begin.AddRefGet();
	*result = reinterpret_cast<XrefChainIteratorHandle>(ptr);
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefChain_Release(XrefChainHandle handle)
{
	XrefChain* chain = reinterpret_cast<XrefChain*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(chain);
	//LOG_SCOPE(chain->GetFile()->GetFilename());

	chain->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
