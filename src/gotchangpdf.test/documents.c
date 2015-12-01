#include "test.h"

error_type process_instruction(ContentInstructionHandle obj, int nested)
{
	ContentInstructionType type;

	print_spaces(nested);
	printf("Content instruction\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_GetType(obj, &type));
	switch (type)
	{
	case ContentInstructionType_Object:
		print_spaces(nested + 1);
		printf("Content object begin\n");
		print_spaces(nested + 1);
		printf("Content object end\n");
		break;
	case ContentInstructionType_Operation:
		print_spaces(nested + 1);
		printf("Content operation begin\n");
		print_spaces(nested + 1);
		printf("Content operation end\n");
		break;
	default:
		print_spaces(nested);
		printf("Unrecognized content instruction type\n");
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	print_spaces(nested);
	printf("Content instruction end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_contents(ContentsHandle obj, int nested)
{
	int i;
	integer_type size;

	print_spaces(nested);
	printf("Contents begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Contents_GetInstructionsSize(obj, &size));

	print_spaces(nested + 1);
	printf("Size: %d\n", size);

	for (i = 0; i < size; ++i) {
		ContentInstructionHandle instruction = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(Contents_GetInstructionAt(obj, i, &instruction));
		RETURN_ERROR_IF_NOT_SUCCESS(process_instruction(instruction, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_Release(instruction));
	}

	print_spaces(nested);
	printf("Contents end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_page(PageObjectHandle obj, int nested)
{
	ContentsHandle contents;

	print_spaces(nested);
	printf("Page begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetContents(obj, &contents));
	RETURN_ERROR_IF_NOT_SUCCESS(process_contents(contents, nested + 1));

	print_spaces(nested);
	printf("Page end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}