#include "precompiled.h"

#include "semantics/objects/catalog.h"

#include "vanillapdf/semantics/c_catalog.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetPages(CatalogHandle* handle, PageTreeHandle** result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputPageTreePtr pages;
		bool found = obj->Pages(pages);
		if (!found) {
			return VANILLAPDF_ERROR_OBJECT_MISSING;
		}

		auto ptr = pages.AddRefGet();
		*result = reinterpret_cast<PageTreeHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetVersion(CatalogHandle* handle, PDFVersion* result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Version version;
		auto contains = obj->Version(version);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
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
			return VANILLAPDF_ERROR_NOT_SUPPORTED;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetExtensions(CatalogHandle* handle, DeveloperExtensionsHandle** result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputDeveloperExtensionsPtr extensions;
		auto contains = obj->Extensions(extensions);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = extensions.AddRefGet();
		*result = reinterpret_cast<DeveloperExtensionsHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetPageLabels(CatalogHandle* handle, PageLabelsHandle** result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputPageLabelsPtr labels;
		auto contains = obj->PageLabels(labels);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = labels.AddRefGet();
		*result = reinterpret_cast<PageLabelsHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetPageLayout(CatalogHandle* handle, PageLayout* result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Catalog::PageLayoutType layout;
		auto contains = obj->PageLayout(layout);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
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
			return VANILLAPDF_ERROR_NOT_SUPPORTED;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetViewerPreferences(CatalogHandle* handle, ViewerPreferencesHandle** result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputViewerPreferencesPtr direct;
		auto contains = obj->ViewerPreferences(direct);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ViewerPreferencesHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetOutlines(CatalogHandle* handle, OutlineHandle** result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputOutlinePtr direct;
		auto contains = obj->Outlines(direct);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<OutlineHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetDestinations(CatalogHandle* handle, NamedDestinationsHandle** result)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputNamedDestinationsPtr direct;
		auto contains = obj->Destinations(direct);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<NamedDestinationsHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_GetAcroForm(CatalogHandle* handle, InteractiveFormHandle** result) {
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OuputInteractiveFormPtr direct;
		auto contains = obj->AcroForm(direct);
		if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<InteractiveFormHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Catalog_Release(CatalogHandle* handle)
{
	return ObjectRelease<Catalog, CatalogHandle*>(handle);
}
