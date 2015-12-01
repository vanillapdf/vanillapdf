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
	LOG_SCOPE(filename);

	try
	{
		DocumentPtr doc(filename);
		auto ptr = doc.AddRefGet();
		*result = reinterpret_cast<DocumentHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_OpenExisting(FileHolderHandle holder_handle, PDocumentHandle result)
{
	FileHolder* holder = reinterpret_cast<FileHolder*>(holder_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(holder);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(file->GetFilename());

	try
	{
		DocumentPtr doc(holder);
		auto ptr = doc.AddRefGet();
		*result = reinterpret_cast<DocumentHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_GetCatalog(DocumentHandle handle, PCatalogHandle result)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(file->GetFilename());

	try
	{
		auto catalog = document->GetDocumentCatalog();
		auto ptr = catalog.AddRefGet();
		*result = reinterpret_cast<CatalogHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_Release(DocumentHandle handle)
{
	Document* obj = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	//LOG_WEAK_FILE_SCOPE(obj->GetFile());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
