#include "precompiled.h"
#include "xref.h"

#include "file.h"

#include "c_xref.h"
#include "c_values.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::files;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Size(XrefHandle handle, out_integer_type result)
{
	Xref* table = reinterpret_cast<Xref*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(table);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(table->GetFile()->GetFilename());

	*result = table->Size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_At(XrefHandle handle, integer_type at, PXrefSubsectionHandle result)
{
	Xref* table = reinterpret_cast<Xref*>(handle);
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
	Xref* table = reinterpret_cast<Xref*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(table);
	LOG_SCOPE(table->GetFile()->GetFilename());

	table->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	entry->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_InUse(XrefEntryHandle handle, out_boolean_type result)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
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
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(entry->GetFile()->GetFilename());

	switch (entry->GetUsage()) {
	case XrefEntry::Usage::Free:
		*result = Free; break;
	case XrefEntry::Usage::Used:
		*result = Used; break;
	case XrefEntry::Usage::Compressed:
		*result = Compressed; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_ToFreeEntry(XrefEntryHandle handle, PXrefFreeEntryHandle result)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
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
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
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
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
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
	LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	*result = section->Index();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Size(XrefSubsectionHandle handle, out_integer_type result)
{
	XrefSubsection* section = reinterpret_cast<XrefSubsection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(section);
	LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	*result = section->Size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_At(XrefSubsectionHandle handle, integer_type at, PXrefEntryHandle result)
{
	XrefSubsection* section = reinterpret_cast<XrefSubsection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(section);
	LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	try {
		auto entry = section->At(at);
		auto ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefEntryHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefSubsection_Release(XrefSubsectionHandle handle)
{
	XrefSubsection* section = reinterpret_cast<XrefSubsection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(section);
	LOG_SCOPE(section->GetParent()->GetFile()->GetFilename());

	section->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
