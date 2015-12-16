#include "test.h"

error_type process_content_object(ContentObjectHandle obj, int nested)
{
	ContentObjectType type;
	ContentObjectTextHandle text_object = NULL;

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
	ContentOperationGenericHandle generic_operation = NULL;
	ContentOperationTextShowHandle textshow_operation = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_GetType(obj, &type));
	switch (type)
	{
	case ContentOperationType_Generic:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToGeneric(obj, &generic_operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_generic(generic_operation, nested + 1));
		break;
	case ContentOperationType_TextShow:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToTextShow(obj, &textshow_operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_textshow(textshow_operation, nested + 1));
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

error_type process_content_operation_textshow(ContentOperationTextShowHandle obj, int nested)
{
	int i = 0;
	int size = 0;
	ContentOperatorHandle oper = NULL;
	StringHandle str = NULL;

	print_spaces(nested);
	printf("Text show content operation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShow_Value(obj, &str));
	RETURN_ERROR_IF_NOT_SUCCESS(process_string(str, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Release(str));

	print_spaces(nested);
	printf("Text show operation end\n");

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
	int i = 0;
	integer_type size = 0;

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
	ContentsHandle contents = NULL;

	print_spaces(nested);
	printf("Page begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetContents(obj, &contents));
	RETURN_ERROR_IF_NOT_SUCCESS(process_contents(contents, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Contents_Release(contents));

	print_spaces(nested);
	printf("Page end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_extensions(DeveloperExtensionsHandle extensions, int nested)
{
	IntegerHandle level = NULL;
	PDFVersion base_version;

	print_spaces(nested);
	printf("Developer extensions begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensions_GetBaseVersion(extensions, &base_version));
	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensions_GetExtensionLevel(extensions, &level));

	RETURN_ERROR_IF_NOT_SUCCESS(process_version(base_version, nested + 1));

	print_spaces(nested + 1);
	printf("Extensions level begin\n");
	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(level, nested + 2));
	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(level));
	print_spaces(nested + 1);
	printf("Extensions level end\n");

	print_spaces(nested);
	printf("Developer extensions end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_catalog(CatalogHandle catalog, int nested)
{
	int i = 0;
	integer_type size = 0;
	PageTreeHandle pages = NULL;
	DeveloperExtensionsHandle extensions = NULL;
	PageLabelsHandle page_labels = NULL;
	PDFVersion version;
	PageLayout page_layout;

	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &pages));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(pages, &size));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(Catalog_GetVersion(catalog, &version), process_version(version, 0));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(Catalog_GetPageLayout(catalog, &page_layout), process_page_layout(page_layout, 0));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetExtensions(catalog, &extensions), process_extensions(extensions, 0), DeveloperExtensions_Release(extensions));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetPageLabels(catalog, &page_labels), process_page_labels(page_labels, size, 0), PageLabels_Release(page_labels));

	for (i = 1; i <= size; ++i)
	{
		PageObjectHandle page = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(pages, i, &page));
		RETURN_ERROR_IF_NOT_SUCCESS(process_page(page, 0));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(pages));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_page_labels(PageLabelsHandle labels, integer_type size, int nested)
{
	int i = 0;

	print_spaces(nested);
	printf("Page labels begin\n");

	for (i = 1; i <= size; ++i)
	{
		boolean_type contains = GOTCHANG_PDF_RV_FALSE;
		PageLabelHandle label = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageLabels_Contains(labels, i, &contains));
		if (GOTCHANG_PDF_RV_TRUE != contains)
			continue;

		RETURN_ERROR_IF_NOT_SUCCESS(PageLabels_At(labels, i, &label));
		RETURN_ERROR_IF_NOT_SUCCESS(process_page_label(label, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(PageLabel_Release(label));
	}

	print_spaces(nested);
	printf("Page labels end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_page_label(PageLabelHandle label, int nested)
{
	StringHandle p = NULL;
	IntegerHandle st = NULL;
	NumberingStyle s;

	print_spaces(nested);
	printf("Page label begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(PageLabel_P(label, &p));
	RETURN_ERROR_IF_NOT_SUCCESS(PageLabel_St(label, &st));
	RETURN_ERROR_IF_NOT_SUCCESS(PageLabel_S(label, &s));

	print_spaces(nested + 1);
	printf("Numbering Style: %d\n", s);
	RETURN_ERROR_IF_NOT_SUCCESS(process_string(p, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(st, nested + 1));

	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Release(p));
	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(st));

	print_spaces(nested);
	printf("Page label end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_page_layout(PageLayout page_layout, int nested)
{
	print_spaces(nested);
	printf("Page layout: %d\n", page_layout);

	return GOTCHANG_PDF_ERROR_SUCCES;
}