#include "test.h"

int main(int argc, char *argv[])
{
	int valid = GOTCHANG_PDF_RV_FALSE;
	FileHandle file = NULL;
	FileHolderHandle file_holder = NULL;
	XrefChainHandle chain = NULL;
	XrefChainIteratorHandle chain_iterator = NULL;

	if (argc != 2)
		return GOTCHANG_PDF_ERROR_GENERAL;

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(803506);
#endif

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(FileHolder_Create(file, &file_holder));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file, file_holder));
	RETURN_ERROR_IF_NOT_SUCCESS(File_XrefChain(file, &chain));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Iterator(chain, &chain_iterator));

	while (GOTCHANG_PDF_ERROR_SUCCES == XrefChainIterator_IsValid(chain_iterator, chain, &valid)
		&& GOTCHANG_PDF_RV_TRUE == valid) {
		XrefHandle xref = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_GetValue(chain_iterator, &xref));

		RETURN_ERROR_IF_NOT_SUCCESS(process_xref(xref, 0));

		RETURN_ERROR_IF_NOT_SUCCESS(Xref_Release(xref));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Next(chain_iterator));
	}

	{
		int i = 0;
		int size = 0;
		DocumentHandle document = NULL;
		CatalogHandle catalog = NULL;
		PageTreeHandle pages = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenExisting(file_holder, &document));
		RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));

		RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &pages));
		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(pages, &size));

		for (i = 1; i <= size; ++i)
		{
			PageObjectHandle page = NULL;
			RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(pages, i, &page));
			RETURN_ERROR_IF_NOT_SUCCESS(process_page(page, 0));
			RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(pages));
		RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));
		RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(document));
	}
	
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Release(chain_iterator));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Release(chain));
	RETURN_ERROR_IF_NOT_SUCCESS(FileHolder_Release(file_holder));

#if (defined(DEBUG) && defined(_MSC_VER))
	_CrtDumpMemoryLeaks();
#endif

	return GOTCHANG_PDF_ERROR_SUCCES;
}
