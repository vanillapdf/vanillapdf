#include "xref.h"
#include "indirect_object.h"

#include "c_xref.h"
#include "c_values.h"

using namespace gotchangpdf;
using namespace gotchangpdf::files;

GOTCHANG_PDF_API int CALLING_CONVENTION Xref_Size(XrefHandle handle)
{
	Xref* table = reinterpret_cast<Xref*>(handle);
	return table->size();
}

GOTCHANG_PDF_API XrefEntryHandle CALLING_CONVENTION Xref_At(XrefHandle handle, int at)
{
	Xref* table = reinterpret_cast<Xref*>(handle);
	XrefEntryPtr entry = table->at(at);
	XrefEntry* ptr = entry.AddRefGet();
	return reinterpret_cast<XrefEntryHandle>(ptr);
}

GOTCHANG_PDF_API void CALLING_CONVENTION Xref_Release(XrefHandle handle)
{
	Xref* table = reinterpret_cast<Xref*>(handle);
	table->Release();
}

GOTCHANG_PDF_API void CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);
	entry->Release();
}

GOTCHANG_PDF_API IndirectHandle CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);

	IndirectObjectPtr indirect = entry->GetReference();
	IndirectObject *ptr = indirect.AddRefGet();
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<IndirectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle)
{
	XrefEntry* entry = reinterpret_cast<XrefEntry*>(handle);

	if (entry->InUse())
		return GOTCHANG_PDF_RV_TRUE;
	else
		return GOTCHANG_PDF_RV_FALSE;
}
