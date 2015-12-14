#include "precompiled.h"
#include "catalog.h"
#include "file.h"

#include "c_catalog.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle, PPageTreeHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);

		try
		{
			auto pages = obj->Pages();
			auto ptr = pages.AddRefGet();
			*result = reinterpret_cast<PageTreeHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle handle, PPDFVersion result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);

		try
		{
			switch (obj->Version()) {
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
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetExtensions(CatalogHandle handle, PDeveloperExtensionsHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			auto extensions = obj->Extensions();
			auto ptr = extensions.AddRefGet();
			*result = reinterpret_cast<DeveloperExtensionsHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPageLabels(CatalogHandle handle, PPageLabelsHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		try
		{
			auto labels = obj->PageLabels();
			auto ptr = labels.AddRefGet();
			*result = reinterpret_cast<PageLabelsHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle handle)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);

		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}
