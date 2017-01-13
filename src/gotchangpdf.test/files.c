#include "test.h"

error_type process_file(FileHandle file, int nested)
{
	XrefChainHandle chain = NULL;
	XrefChainIteratorHandle chain_iterator = NULL;
	boolean_type valid = GOTCHANG_PDF_RV_FALSE;

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

	RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Release(chain_iterator));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Release(chain));

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_xref(XrefHandle xref, int nested)
{
	XrefIteratorHandle xref_iterator = NULL;
	boolean_type valid = GOTCHANG_PDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_Iterator(xref, &xref_iterator));

	while (GOTCHANG_PDF_ERROR_SUCCES == XrefIterator_IsValid(xref_iterator, xref, &valid)
		&& GOTCHANG_PDF_RV_TRUE == valid) {

		XrefEntryType type;
		XrefEntryHandle entry = NULL;
		ObjectHandle obj = NULL;
		XrefFreeEntryHandle free_entry = NULL;
		XrefCompressedEntryHandle compressed_entry = NULL;
		XrefUsedEntryHandle used_entry = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_GetValue(xref_iterator, &entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Type(entry, &type));

		switch (type) {
		case XrefEntryType_Free:
			RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToFreeEntry(entry, &free_entry));
			break;
		case XrefEntryType_Used:
			RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToUsedEntry(entry, &used_entry));
			RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_Reference(used_entry, &obj));
			RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
			break;
		case XrefEntryType_Compressed:
			RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToCompressedEntry(entry, &compressed_entry));
			RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_Reference(compressed_entry, &obj));
			RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
			break;
		case XrefEntryType_Null:
			printf("Missing xref entry\n");
			return GOTCHANG_PDF_TEST_ERROR_FAILURE;
		default:
			printf("Unknown xref entry type\n");
			return GOTCHANG_PDF_TEST_ERROR_FAILURE;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Release(entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Next(xref_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Release(xref_iterator));

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}