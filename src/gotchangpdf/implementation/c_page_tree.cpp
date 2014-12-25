#include "page_tree.h"
#include "c_page_tree.h"

using namespace gotchangpdf::documents;

GOTCHANG_PDF_API long long CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle handle)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	return obj->PageCount();
}

GOTCHANG_PDF_API PageObjectHandle CALLING_CONVENTION PageTree_GetPage(PageTreeHandle handle, int at)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	auto page = obj->Page(at);
	return reinterpret_cast<PageObjectHandle>(page.AddRefGet());
}

GOTCHANG_PDF_API void CALLING_CONVENTION PageTree_Release(PageTreeHandle handle)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	obj->Release();
}