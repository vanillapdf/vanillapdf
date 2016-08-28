#include "precompiled.h"
#include "document.h"

#include "c_document.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_OpenNew(string_type filename, PDocumentHandle result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		std::string name(filename);
		DocumentPtr doc(name);
		auto ptr = doc.AddRefGet();
		*result = reinterpret_cast<DocumentHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_OpenExisting(FileHandle holder_handle, PDocumentHandle result)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(holder_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		DocumentPtr doc(holder);
		auto ptr = doc.AddRefGet();
		*result = reinterpret_cast<DocumentHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_Save(DocumentHandle handle, string_type filename)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);

	try
	{
		document->Save(filename);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_SaveIncremental(DocumentHandle handle, string_type filename)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);

	try
	{
		document->SaveIncremental(filename);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_GetCatalog(DocumentHandle handle, PCatalogHandle result)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto catalog = document->GetDocumentCatalog();
		auto ptr = catalog.AddRefGet();
		*result = reinterpret_cast<CatalogHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_GetDocumentInfo(DocumentHandle handle, PDocumentInfoHandle result)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDocumentInfoPtr info;
		bool found = document->GetDocumentInfo(info);
		if (!found) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = info.AddRefGet();
		*result = reinterpret_cast<DocumentInfoHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_Release(DocumentHandle handle)
{
	return ObjectRelease<Document, DocumentHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_AppendContent(DocumentHandle handle, DocumentHandle source_handle)
{
	Document* document = reinterpret_cast<Document*>(handle);
	Document* source = reinterpret_cast<Document*>(source_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source);

	try
	{
		document->AppendContent(*source);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}
