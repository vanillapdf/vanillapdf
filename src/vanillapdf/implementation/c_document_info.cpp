#include "precompiled.h"
#include "semantics/objects/document_info.h"
#include "semantics/objects/date.h"

#include "vanillapdf/c_document_info.h"
#include "c_helper.h"

using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetTitle(DocumentInfoHandle handle, StringObjectHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr title;
		auto contains = obj->Title(title);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = title.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetAuthor(DocumentInfoHandle handle, StringObjectHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr author;
		auto contains = obj->Author(author);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = author.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetSubject(DocumentInfoHandle handle, StringObjectHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr subject;
		auto contains = obj->Subject(subject);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = subject.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetKeywords(DocumentInfoHandle handle, StringObjectHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr keywords;
		auto contains = obj->Keywords(keywords);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = keywords.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreator(DocumentInfoHandle handle, StringObjectHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr creator;
		auto contains = obj->Creator(creator);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = creator.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetProducer(DocumentInfoHandle handle, StringObjectHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputStringObjectPtr producer;
		auto contains = obj->Producer(producer);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = producer.AddRefGet();
		*result = reinterpret_cast<StringObjectHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreationDate(DocumentInfoHandle handle, DateHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDatePtr date;
		auto contains = obj->CreationDate(date);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = date.AddRefGet();
		*result = reinterpret_cast<DateHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetModificationDate(DocumentInfoHandle handle, DateHandle* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDatePtr date;
		auto contains = obj->ModificationDate(date);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = date.AddRefGet();
		*result = reinterpret_cast<DateHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetTrapped(DocumentInfoHandle handle, DocumentTrappedType* result)
{
	DocumentInfo* obj = reinterpret_cast<DocumentInfo*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		vanillapdf::semantics::DocumentTrapped trapped;
		auto contains = obj->Trapped(trapped);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;

		switch (trapped)
		{
		case vanillapdf::semantics::DocumentTrapped::Unknown:
			*result = DocumentTrappedType_Unknown; break;
		case vanillapdf::semantics::DocumentTrapped::True:
			*result = DocumentTrappedType_True; break;
		case vanillapdf::semantics::DocumentTrapped::False:
			*result = DocumentTrappedType_False; break;
		default:
			return VANILLAPDF_ERROR_GENERAL;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_Release(DocumentInfoHandle handle)
{
	return ObjectRelease<DocumentInfo, DocumentInfoHandle>(handle);
}