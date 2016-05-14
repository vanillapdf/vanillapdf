#include "precompiled.h"
#include "document_info.h"
#include "date.h"

#include "c_document_info.h"
#include "c_helper.h"

using namespace gotchangpdf::syntax;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTitle(DocumentInfoHandle handle, PStringHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr title;
		auto contains = obj->Title(title);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = title.AddRefGet();
		*result = reinterpret_cast<StringHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetAuthor(DocumentInfoHandle handle, PStringHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr author;
		auto contains = obj->Author(author);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = author.AddRefGet();
		*result = reinterpret_cast<StringHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetSubject(DocumentInfoHandle handle, PStringHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr subject;
		auto contains = obj->Subject(subject);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = subject.AddRefGet();
		*result = reinterpret_cast<StringHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetKeywords(DocumentInfoHandle handle, PStringHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr keywords;
		auto contains = obj->Keywords(keywords);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = keywords.AddRefGet();
		*result = reinterpret_cast<StringHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreator(DocumentInfoHandle handle, PStringHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr creator;
		auto contains = obj->Creator(creator);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = creator.AddRefGet();
		*result = reinterpret_cast<StringHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetProducer(DocumentInfoHandle handle, PStringHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr producer;
		auto contains = obj->Producer(producer);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = producer.AddRefGet();
		*result = reinterpret_cast<StringHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreationDate(DocumentInfoHandle handle, PDateHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDatePtr date;
		auto contains = obj->CreationDate(date);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = date.AddRefGet();
		*result = reinterpret_cast<DateHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetModificationDate(DocumentInfoHandle handle, PDateHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDatePtr date;
		auto contains = obj->ModificationDate(date);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = date.AddRefGet();
		*result = reinterpret_cast<DateHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTrapped(DocumentInfoHandle handle, PNameHandle result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputNameObjectPtr trapped;
		auto contains = obj->Trapped(trapped);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = trapped.AddRefGet();
		*result = reinterpret_cast<NameHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_Release(DocumentInfoHandle handle)
{
	return ObjectRelease<DocumentInfo, DocumentInfoHandle>(handle);
}