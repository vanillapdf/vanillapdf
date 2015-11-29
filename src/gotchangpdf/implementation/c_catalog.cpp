#include "precompiled.h"
#include "catalog.h"

#include "c_catalog.h"
#include "c_helper.h"

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
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
