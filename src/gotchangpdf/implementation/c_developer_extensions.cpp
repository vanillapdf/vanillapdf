#include "precompiled.h"
#include "developer_extensions.h"
#include "file.h"

#include "gotchangpdf/c_developer_extensions.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_GetBaseVersion(DeveloperExtensionHandle handle, PPDFVersion result)
{
	DeveloperExtension* obj = reinterpret_cast<DeveloperExtension*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		switch (obj->BaseVersion()) {
		case Version::PDF10:
			*result = PDFVersion_10; break;
		case Version::PDF11:
			*result = PDFVersion_11; break;
		case Version::PDF12:
			*result = PDFVersion_12; break;
		case Version::PDF13:
			*result = PDFVersion_13; break;
		case Version::PDF14:
			*result = PDFVersion_14; break;
		case Version::PDF15:
			*result = PDFVersion_15; break;
		case Version::PDF16:
			*result = PDFVersion_16; break;
		case Version::PDF17:
			*result = PDFVersion_17; break;
		default:
			return GOTCHANG_PDF_ERROR_NOT_SUPPORTED;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_GetExtensionLevel(DeveloperExtensionHandle handle, PIntegerObjectHandle result)
{
	DeveloperExtension* obj = reinterpret_cast<DeveloperExtension*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto level = obj->ExtensionLevel();
		auto ptr = level.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtension_Release(DeveloperExtensionHandle handle)
{
	return ObjectRelease<DeveloperExtension, DeveloperExtensionHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Iterator(DeveloperExtensionsHandle handle, PDeveloperExtensionsIteratorHandle result)
{
	DeveloperExtensions* obj = reinterpret_cast<DeveloperExtensions*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto it = obj->Begin();
		auto ptr = it.AddRefGet();
		*result = reinterpret_cast<DeveloperExtensionsIteratorHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensions_Release(DeveloperExtensionsHandle handle)
{
	return ObjectRelease<DeveloperExtensions, DeveloperExtensionsHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetKey(DeveloperExtensionsIteratorHandle handle, PNameObjectHandle result)
{
	DeveloperExtensions::Iterator* obj = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto name = obj->First();
		auto ptr = name.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_GetValue(DeveloperExtensionsIteratorHandle handle, PDeveloperExtensionHandle result)
{
	DeveloperExtensions::Iterator* obj = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto extension = obj->Second();
		auto ptr = extension.AddRefGet();
		*result = reinterpret_cast<DeveloperExtensionHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_IsValid(DeveloperExtensionsIteratorHandle handle, DeveloperExtensionsHandle parent, out_boolean_type result)
{
	DeveloperExtensions::Iterator* iterator = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	DeveloperExtensions* dictionary = reinterpret_cast<DeveloperExtensions*>(parent);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		if (*dictionary->End() == *iterator)
			*result = GOTCHANG_PDF_FALSE;
		else
			*result = GOTCHANG_PDF_TRUE;

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Next(DeveloperExtensionsIteratorHandle handle)
{
	DeveloperExtensions::Iterator* obj = reinterpret_cast<DeveloperExtensions::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		++(*obj);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DeveloperExtensionsIterator_Release(DeveloperExtensionsIteratorHandle handle)
{
	return ObjectRelease<DeveloperExtensions::Iterator, DeveloperExtensionsIteratorHandle>(handle);
}