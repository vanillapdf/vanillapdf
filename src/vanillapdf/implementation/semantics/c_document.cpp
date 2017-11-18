#include "precompiled.h"
#include "semantics/objects/document.h"

#include "vanillapdf/semantics/c_document.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Document_Open(string_type filename, DocumentHandle* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		std::string name(filename);
		DocumentPtr doc = Document::Open(name);
		auto ptr = doc.AddRefGet();
		*result = reinterpret_cast<DocumentHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_Create(string_type filename, DocumentHandle* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		std::string name(filename);
		DocumentPtr doc = Document::Create(name);
		auto ptr = doc.AddRefGet();
		*result = reinterpret_cast<DocumentHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_OpenFile(FileHandle holder_handle, DocumentHandle* result)
{
	File* file = reinterpret_cast<File*>(holder_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		DocumentPtr doc = Document::OpenFile(file);
		auto ptr = doc.AddRefGet();
		*result = reinterpret_cast<DocumentHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_Save(DocumentHandle handle, string_type filename)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);

	try
	{
		document->Save(filename);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_SaveIncremental(DocumentHandle handle, string_type filename)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);

	try
	{
		document->SaveIncremental(filename);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_GetCatalog(DocumentHandle handle, CatalogHandle* result)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputCatalogPtr catalog;
		bool found = document->GetDocumentCatalog(catalog);
		if (!found) {
			return VANILLAPDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = catalog.AddRefGet();
		*result = reinterpret_cast<CatalogHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_GetDocumentInfo(DocumentHandle handle, DocumentInfoHandle* result)
{
	Document* document = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDocumentInfoPtr info;
		bool found = document->GetDocumentInfo(info);
		if (!found) {
			return VANILLAPDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = info.AddRefGet();
		*result = reinterpret_cast<DocumentInfoHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_Release(DocumentHandle handle)
{
	return ObjectRelease<Document, DocumentHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION Document_AppendDocument(DocumentHandle handle, DocumentHandle source_handle)
{
	Document* document = reinterpret_cast<Document*>(handle);
	Document* source = reinterpret_cast<Document*>(source_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(source);

	try
	{
		document->AppendDocument(source);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}
