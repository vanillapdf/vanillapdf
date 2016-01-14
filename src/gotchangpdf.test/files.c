#include "test.h"

error_type process_xref(XrefHandle xref, int nested)
{
	integer_type i = 0, size = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_Size(xref, &size));

	for (i = 0; i < size; ++i) {
		uinteger_type j = 0, subsection_size = 0;
		XrefSubsectionHandle subsection = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(Xref_At(xref, i, &subsection));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefSubsection_Size(subsection, &subsection_size));

		for (j = 0; j < subsection_size; ++j) {
			XrefEntryType type;
			XrefEntryHandle entry = NULL;
			ObjectHandle obj = NULL;
			XrefFreeEntryHandle free_entry = NULL;
			XrefCompressedEntryHandle compressed_entry = NULL;
			XrefUsedEntryHandle used_entry = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(XrefSubsection_At(subsection, j, &entry));
			RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Type(entry, &type));

			switch (type) {
			case XrefEntryFree:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToFreeEntry(entry, &free_entry));
				break;
			case XrefEntryUsed:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToUsedEntry(entry, &used_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_Reference(used_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, nested + 1));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				break;
			case XrefEntryCompressed:
				RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToCompressedEntry(entry, &compressed_entry));
				RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_Reference(compressed_entry, &obj));
				RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, nested + 1));
				RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
				break;
			case XrefEntryNull:
				printf("Missing xref entry\n");
				return GOTCHANG_PDF_ERROR_GENERAL;
				break;
			default:
				printf("Unknown xref entry type\n");
				return GOTCHANG_PDF_ERROR_GENERAL;
			}

			RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_Release(entry));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(XrefSubsection_Release(subsection));
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}