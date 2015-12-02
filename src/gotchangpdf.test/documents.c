#include "test.h"

error_type process_content_object(ContentObjectHandle obj, int nested)
{
	ContentObjectType type;
	ContentObjectTextHandle text_object;

	RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_GetType(obj, &type));
	switch (type)
	{
	case ContentObjectType_Text:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_ToText(obj, &text_object));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_object_text(text_object, nested + 1));
		break;
	default:
		print_spaces(nested + 1);
		printf("Unrecognized content object type\n");
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_object_text(ContentObjectTextHandle obj, int nested)
{
	int i = 0;
	int size = 0;

	print_spaces(nested);
	printf("Text object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationsSize(obj, &size));

	print_spaces(nested + 1);
	printf("Operations: %d\n", size);

	for (i = 0; i < size; ++i)
	{
		ContentOperationHandle operation = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationAt(obj, i, &operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation(operation, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_Release(operation));
	}

	print_spaces(nested);
	printf("Text object end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_operation(ContentOperationHandle obj, int nested)
{
	ContentOperationType type;
	ContentOperationGenericHandle generic_operation;

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_GetType(obj, &type));
	switch (type)
	{
	case ContentOperationType_Generic:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToGeneric(obj, &generic_operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_generic(generic_operation, nested + 1));
		break;
	default:
		print_spaces(nested + 1);
		printf("Unrecognized content operation type\n");
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_operation_generic(ContentOperationGenericHandle obj, int nested)
{
	int i = 0;
	int size = 0;
	ContentOperatorHandle oper = NULL;

	print_spaces(nested);
	printf("Generic content operation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_GetOperator(obj, &oper));
	RETURN_ERROR_IF_NOT_SUCCESS(process_content_operator(oper, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperator_Release(oper));

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_GetOperandsSize(obj, &size));

	print_spaces(nested + 1);
	printf("Operands: %d\n", size);

	for (i = 0; i < size; ++i)
	{
		ObjectHandle operand = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_GetOperandAt(obj, i, &operand));
		RETURN_ERROR_IF_NOT_SUCCESS(process_object(operand, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(operand));
	}

	print_spaces(nested);
	printf("Generic content operation end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_operator(ContentOperatorHandle obj, int nested)
{
	ContentOperatorType type;
	BufferHandle buffer = NULL;

	print_spaces(nested);
	printf("Content operator begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperator_GetType(obj, &type));

	print_spaces(nested + 1);
	printf("Type: %d\n", type);

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperator_GetValue(obj, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	printf("Content operator end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_instruction(ContentInstructionHandle obj, int nested)
{
	ContentInstructionType type;
	ContentObjectHandle object_handle = NULL;
	ContentOperationHandle operation_handle = NULL;

	print_spaces(nested);
	printf("Content instruction\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_GetType(obj, &type));
	switch (type)
	{
	case ContentInstructionType_Object:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_ToObject(obj, &object_handle));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_object(object_handle, nested + 1));

		break;
	case ContentInstructionType_Operation:

		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_ToOperation(obj, &operation_handle));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation(operation_handle, nested + 1));

		break;
	default:
		print_spaces(nested + 1);
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
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_instruction(instruction, nested + 1));
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
	RETURN_ERROR_IF_NOT_SUCCESS(Contents_Release(contents));

	print_spaces(nested);
	printf("Page end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}