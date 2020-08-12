#include "test.h"

//! [Enumerate xref]
error_type process_file(FileHandle* file, int nested) {
	XrefChainHandle* chain = NULL;
	XrefChainIteratorHandle* chain_iterator = NULL;
	boolean_type valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(File_XrefChain(file, &chain));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Iterator(chain, &chain_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == XrefChain_IsIteratorValid(chain, chain_iterator, &valid)
		&& VANILLAPDF_RV_TRUE == valid) {
		XrefHandle* xref = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_GetValue(chain_iterator, &xref));
		RETURN_ERROR_IF_NOT_SUCCESS(process_xref(xref, nested));
		RETURN_ERROR_IF_NOT_SUCCESS(Xref_Release(xref));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Next(chain_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Release(chain_iterator));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Release(chain));

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
//! [Enumerate xref]

error_type process_xref(XrefHandle* xref, int nested) {
	XrefIteratorHandle* xref_iterator = NULL;
	boolean_type valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_Iterator(xref, &xref_iterator));

	while (VANILLAPDF_ERROR_SUCCESS == Xref_IsIteratorValid(xref, xref_iterator, &valid)
		&& VANILLAPDF_RV_TRUE == valid) {

		XrefEntryType type;
		XrefEntryHandle* entry = NULL;
		ObjectHandle* obj = NULL;
		XrefFreeEntryHandle* free_entry = NULL;
		XrefCompressedEntryHandle* compressed_entry = NULL;
		XrefUsedEntryHandle* used_entry = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_GetValue(xref_iterator, &entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetType(entry, &type));

		switch (type) {
			case XrefEntryType_Free:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefFreeEntry_FromEntry(entry, &free_entry));
				break;
			case XrefEntryType_Used:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_FromEntry(entry, &used_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_GetReference(used_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, nested + 1));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				break;
			case XrefEntryType_Compressed:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_FromEntry(entry, &compressed_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_GetReference(compressed_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, nested + 1));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				break;
			case XrefEntryType_Null:
				print_text("Missing xref entry\n");
				return VANILLAPDF_TEST_ERROR_FAILURE;
			default:
				print_text("Unknown xref entry type\n");
				return VANILLAPDF_TEST_ERROR_FAILURE;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Release(entry));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Next(xref_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Release(xref_iterator));

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
