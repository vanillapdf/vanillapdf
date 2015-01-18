#include "xref.h"

#include "object_visitors.h"

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

	try
	{
		*result = table->size();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_At(XrefHandle handle, integer_type at, PXrefEntryHandle result)
{
	Xref* table = reinterpret_cast<Xref*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(table);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		XrefEntryPtr entry = table->at(at);
		XrefEntry* ptr = entry.AddRefGet();
		*result = reinterpret_cast<XrefEntryHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle handle)
{
	Xref* table = reinterpret_cast<Xref*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(table);

	try
	{
		table->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);

	try
	{
		entry->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle, PObjectHandle result)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = entry->GetReference();
		ObjectBaseAddRefVisitor visitor;
		auto base = direct.apply_visitor(visitor);

		*result = reinterpret_cast<ObjectHandle>(base);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle, out_boolean_type result)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(entry);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		if (entry->InUse())
			*result = GOTCHANG_PDF_RV_TRUE;
		else
			*result = GOTCHANG_PDF_RV_FALSE;

		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
