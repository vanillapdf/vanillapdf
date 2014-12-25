#include "catalog.h"
#include "c_catalog.h"

using namespace gotchangpdf::documents;

GOTCHANG_PDF_API PageTreeHandle CALLING_CONVENTION Catalog_GetPages(CatalogHandle handle)
{
	Catalog* obj = reinterpret_cast<Catalog*>(handle);
	auto pages = obj->Pages();
	return reinterpret_cast<PageTreeHandle>(pages.AddRefGet());
}