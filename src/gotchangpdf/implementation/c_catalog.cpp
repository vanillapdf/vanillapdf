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
		auto pages = obj->Pages();
		auto ptr = pages.AddRefGet();
		*result = reinterpret_cast<PageTreeHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle handle, PPDFVersion result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Version version;
		auto contains = obj->Version(version);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		switch (version) {
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetExtensions(CatalogHandle handle, PDeveloperExtensionsHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDeveloperExtensionsPtr extensions;
		auto contains = obj->Extensions(extensions);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = extensions.AddRefGet();
		*result = reinterpret_cast<DeveloperExtensionsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPageLabels(CatalogHandle handle, PPageLabelsHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputPageLabelsPtr labels;
		auto contains = obj->PageLabels(labels);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = labels.AddRefGet();
		*result = reinterpret_cast<PageLabelsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetPageLayout(CatalogHandle handle, PPageLayout result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Catalog::PageLayoutType layout;
		auto contains = obj->PageLayout(layout);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		switch (layout) {
		case semantics::Catalog::PageLayoutType::SinglePage:
			*result = PageLayout_SinglePage; break;
		case semantics::Catalog::PageLayoutType::OneColumn:
			*result = PageLayout_OneColumn; break;
		case semantics::Catalog::PageLayoutType::TwoColumnLeft:
			*result = PageLayout_TwoColumnLeft; break;
		case semantics::Catalog::PageLayoutType::TwoColumnRight:
			*result = PageLayout_TwoColumnRight; break;
		case semantics::Catalog::PageLayoutType::TwoPageLeft:
			*result = PageLayout_TwoPageLeft; break;
		case semantics::Catalog::PageLayoutType::TwoPageRight:
			*result = PageLayout_TwoPageRight; break;
		default:
			return GOTCHANG_PDF_ERROR_NOT_SUPPORTED;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetViewerPreferences(CatalogHandle handle, PViewerPreferencesHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputViewerPreferencesPtr direct;
		auto contains = obj->ViewerPreferences(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ViewerPreferencesHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetOutlines(CatalogHandle handle, POutlineHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlinePtr direct;
		auto contains = obj->Outlines(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_GetDestinations(CatalogHandle handle, PNamedDestinationsHandle result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputNamedDestinationsPtr direct;
		auto contains = obj->Destinations(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NamedDestinationsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle handle)
{
	return ObjectRelease<Catalog, CatalogHandle>(handle);
}
