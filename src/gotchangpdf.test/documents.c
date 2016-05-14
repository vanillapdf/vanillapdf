#include "test.h"

error_type process_document(DocumentHandle document, int nested)
{
	CatalogHandle catalog = NULL;
	DocumentInfoHandle info = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));
	RETURN_ERROR_IF_NOT_SUCCESS(process_catalog(catalog, 0));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetDocumentInfo(document, &info));
	RETURN_ERROR_IF_NOT_SUCCESS(process_document_info(info, 0));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentInfo_Release(info));

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_object(ContentObjectHandle obj, int nested)
{
	ContentObjectType type;
	ContentObjectTextHandle text_object = NULL;
	ContentObjectInlineImageHandle image_object = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_GetType(obj, &type));
	switch (type)
	{
	case ContentObjectType_Text:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_ToText(obj, &text_object));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_object_text(text_object, nested + 1));
		break;
	case ContentObjectType_InlineImage:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_ToInlineImage(obj, &image_object));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_object_inline_image(image_object, nested + 1));
		break;
	default:
		print_spaces(nested + 1);
		printf("Unrecognized content object type\n");
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_object_inline_image(ContentObjectInlineImageHandle obj, int nested)
{
	DictionaryHandle dictionary = NULL;
	BufferHandle data = NULL;

	print_spaces(nested);
	printf("Inline image object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetDictionary(obj, &dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetData(obj, &data));

	RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(data, nested + 1));

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(data));

	print_spaces(nested);
	printf("Inline image object end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_object_text(ContentObjectTextHandle obj, int nested)
{
	integer_type i = 0;
	integer_type size = 0;

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
	ContentOperationTextShowHandle text_show_operation = NULL;
	ContentOperationEndTextHandle end_text_operation = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_GetType(obj, &type));
	switch (type)
	{
	case ContentOperationType_Generic:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToGeneric(obj, &generic_operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_generic(generic_operation, nested + 1));
		break;
	case ContentOperationType_TextShow:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToTextShow(obj, &text_show_operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_textshow(text_show_operation, nested + 1));
		break;
	case ContentOperationType_EndText:
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToEndText(obj, &end_text_operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_endtext(end_text_operation, nested + 1));
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
	integer_type i = 0;
	integer_type size = 0;
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

error_type process_content_operation_endtext(ContentOperationEndTextHandle obj, int nested)
{
	print_spaces(nested);
	printf("Content operation: ET\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_content_operation_textshow(ContentOperationTextShowHandle obj, int nested)
{
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
	integer_type i = 0;
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

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageObject_GetContents(obj, &contents),
		process_contents(contents, nested + 1),
		Contents_Release(contents));

	print_spaces(nested);
	printf("Page end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_extensions(DeveloperExtensionsHandle extensions, int nested)
{
	boolean_type boolean = GOTCHANG_PDF_FALSE;
	DeveloperExtensionsIteratorHandle iterator = NULL;

	print_spaces(nested);
	printf("Developer extensions begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensions_Iterator(extensions, &iterator));
	while (GOTCHANG_PDF_ERROR_SUCCES == DeveloperExtensionsIterator_IsValid(iterator, extensions, &boolean)
		&& GOTCHANG_PDF_TRUE == boolean)
	{
		NameHandle key = NULL;
		DeveloperExtensionHandle value = NULL;

		print_spaces(nested);
		printf("Pair:\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_GetKey(iterator, &key));
		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_GetValue(iterator, &value));

		RETURN_ERROR_IF_NOT_SUCCESS(process_name(key, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(process_extension(value, nested + 1));

		RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(key));
		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtension_Release(value));

		print_spaces(nested);
		printf("EndPair\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_Next(iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_Release(iterator));

	print_spaces(nested);
	printf("Developer extensions End\n");
	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_extension(DeveloperExtensionHandle extension, int nested)
{
	IntegerHandle level = NULL;
	PDFVersion base_version;

	print_spaces(nested);
	printf("Developer extensions begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtension_GetBaseVersion(extension, &base_version));
	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtension_GetExtensionLevel(extension, &level));

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
	integer_type i = 0;
	integer_type size = 0;
	PageTreeHandle pages = NULL;
	DeveloperExtensionsHandle extensions = NULL;
	PageLabelsHandle page_labels = NULL;
	ViewerPreferencesHandle viewer_preferences = NULL;
	OutlineHandle outlines = NULL;
	PDFVersion version;
	PageLayout page_layout;

	print_spaces(nested);
	printf("Document catalog begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &pages));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(pages, &size));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(Catalog_GetVersion(catalog, &version), process_version(version, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(Catalog_GetPageLayout(catalog, &page_layout), process_page_layout(page_layout, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetExtensions(catalog, &extensions),
		process_extensions(extensions, nested + 1),
		DeveloperExtensions_Release(extensions));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetPageLabels(catalog, &page_labels),
		process_page_labels(page_labels, size, nested + 1),
		PageLabels_Release(page_labels));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetViewerPreferences(catalog, &viewer_preferences),
		process_viewer_preferences(viewer_preferences, nested + 1),
		ViewerPreferences_Release(viewer_preferences));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetOutlines(catalog, &outlines),
		process_outline(outlines, nested + 1),
		Outline_Release(outlines));

	for (i = 1; i <= size; ++i)
	{
		PageObjectHandle page = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(pages, i, &page));
		RETURN_ERROR_IF_NOT_SUCCESS(process_page(page, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(pages));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));

	print_spaces(nested);
	printf("Document catalog begin\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_document_info(DocumentInfoHandle obj, int nested)
{
	StringHandle title = NULL;
	StringHandle author = NULL;
	StringHandle subject = NULL;
	StringHandle keywords = NULL;
	StringHandle creator = NULL;
	StringHandle producer = NULL;
	DateHandle creation_date = NULL;
	DateHandle modification_date = NULL;
	NameHandle trapped = NULL;

	print_spaces(nested);
	printf("Document info begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetTitle(obj, &title),
		process_string(title, nested + 1),
		StringObject_Release(title));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetAuthor(obj, &author),
		process_string(author, nested + 1),
		StringObject_Release(author));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetSubject(obj, &subject),
		process_string(subject, nested + 1),
		StringObject_Release(subject));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetKeywords(obj, &keywords),
		process_string(keywords, nested + 1),
		StringObject_Release(keywords));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetCreator(obj, &creator),
		process_string(creator, nested + 1),
		StringObject_Release(creator));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetProducer(obj, &producer),
		process_string(producer, nested + 1),
		StringObject_Release(producer));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetCreationDate(obj, &creation_date),
		process_date(creation_date, nested + 1),
		Date_Release(creation_date));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetModificationDate(obj, &modification_date),
		process_date(modification_date, nested + 1),
		Date_Release(modification_date));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DocumentInfo_GetTrapped(obj, &trapped),
		process_name(trapped, nested + 1),
		NameObject_Release(trapped));

	print_spaces(nested);
	printf("Document info end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_date(DateHandle obj, int nested)
{
	integer_type year = 0;
	integer_type month = 0;
	integer_type day = 0;
	integer_type hour = 0;
	integer_type minute = 0;
	integer_type second = 0;
	DateTimezoneType timezone;
	integer_type hour_offset = 0;
	integer_type minute_offset = 0;

	print_spaces(nested);
	printf("Date begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetYear(obj, &year));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetMonth(obj, &month));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetDay(obj, &day));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetHour(obj, &hour));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetMinute(obj, &minute));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetSecond(obj, &second));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetTimezone(obj, &timezone));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetHourOffset(obj, &hour_offset));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_GetMinuteOffset(obj, &minute_offset));

	print_spaces(nested + 1);
	if (timezone == DateTimezoneType_UTC) {
		printf("%04d-%02d-%02d %02d:%02d:%02d\n",
			year, month, day,
			hour, minute, second);
	}
	else {
		printf("%04d-%02d-%02d %02d:%02d:%02d %c%02d:%02d\n",
			year, month, day,
			hour, minute, second,
			timezone, hour_offset, minute_offset);
	}

	print_spaces(nested);
	printf("Date outline end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_outline(OutlineHandle outline, int nested)
{
	OutlineItemHandle first = NULL;
	OutlineItemHandle last = NULL;
	IntegerHandle count = NULL;

	print_spaces(nested);
	printf("Document outline begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Outline_GetFirst(outline, &first),
		process_outline_item(first, nested + 1),
		OutlineItem_Release(first));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Outline_GetLast(outline, &last),
		process_outline_item(last, nested + 1),
		OutlineItem_Release(last));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Outline_GetCount(outline, &count),
		process_integer(count, nested + 1),
		IntegerObject_Release(count));

	print_spaces(nested);
	printf("Document outline end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_outline_item(OutlineItemHandle outline, int nested)
{
	OutlineBaseHandle parent = NULL;
	OutlineItemHandle first = NULL;
	OutlineItemHandle last = NULL;
	OutlineItemHandle next = NULL;
	OutlineItemHandle prev = NULL;
	IntegerHandle count = NULL;
	StringHandle title = NULL;
	OutlineItemColorHandle color = NULL;
	OutlineItemFlagsHandle flags = NULL;

	print_spaces(nested);
	printf("Document outline begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItem_GetTitle(outline, &title));
	RETURN_ERROR_IF_NOT_SUCCESS(process_string(title, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Release(title));

	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItem_GetParent(outline, &parent));
	RETURN_ERROR_IF_NOT_SUCCESS(process_outline_base(parent, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(OutlineBase_Release(parent));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetFirst(outline, &first),
		process_outline_item(first, nested + 1),
		OutlineItem_Release(first));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetLast(outline, &last),
		process_outline_item(last, nested + 1),
		OutlineItem_Release(last));

	//RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetPrev(outline, &prev),
	//	process_outline_item(prev, nested + 1),
	//	OutlineItem_Release(prev));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetNext(outline, &next),
		process_outline_item(next, nested + 1),
		OutlineItem_Release(next));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetCount(outline, &count),
		process_integer(count, nested + 1),
		IntegerObject_Release(count));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetColor(outline, &color),
		process_outline_item_color(color, nested + 1),
		OutlineItemColor_Release(color));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetFlags(outline, &flags),
		process_outline_item_flags(flags, nested + 1),
		OutlineItemFlags_Release(flags));

	print_spaces(nested);
	printf("Document outline end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_outline_base(OutlineBaseHandle outline, int nested)
{
	OutlineType type;

	print_spaces(nested);
	printf("Outline base begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(OutlineBase_GetType(outline, &type));

	print_spaces(nested + 1);
	printf("Type: %d\n", type);

	print_spaces(nested);
	printf("Outline base end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_outline_item_color(OutlineItemColorHandle obj, int nested)
{
	IntegerHandle red = NULL;
	IntegerHandle green = NULL;
	IntegerHandle blue = NULL;

	print_spaces(nested);
	printf("Outline item color begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItemColor_GetRed(obj, &red));
	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItemColor_GetGreen(obj, &green));
	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItemColor_GetBlue(obj, &blue));

	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(red, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(green, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(blue, nested + 1));

	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(red));
	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(green));
	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(blue));

	print_spaces(nested);
	printf("Outline item color end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_outline_item_flags(OutlineItemFlagsHandle obj, int nested)
{
	boolean_type is_italic = GOTCHANG_PDF_FALSE;
	boolean_type is_bold = GOTCHANG_PDF_FALSE;

	print_spaces(nested);
	printf("Outline item flags begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItemFlags_IsItalic(obj, &is_italic));
	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItemFlags_IsBold(obj, &is_bold));

	print_spaces(nested + 1);
	printf("Italic: %d\n", is_italic);

	print_spaces(nested + 1);
	printf("Bold: %d\n", is_bold);

	print_spaces(nested);
	printf("Outline item flags end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_page_labels(PageLabelsHandle labels, integer_type size, int nested)
{
	integer_type i = 0;

	print_spaces(nested);
	printf("Page labels begin\n");

	for (i = 1; i <= size; ++i)
	{
		boolean_type contains = GOTCHANG_PDF_FALSE;
		PageLabelHandle label = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageLabels_Contains(labels, i, &contains));
		if (GOTCHANG_PDF_TRUE != contains)
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

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageLabel_P(label, &p), process_string(p, nested + 1), StringObject_Release(p));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageLabel_St(label, &st), process_integer(st, nested + 1), IntegerObject_Release(st));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(PageLabel_S(label, &s), proces_numbering_style(s, nested + 1));

	print_spaces(nested);
	printf("Page label end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type proces_numbering_style(NumberingStyle style, int nested)
{
	print_spaces(nested + 1);
	printf("Numbering Style: %d\n", style);

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_page_layout(PageLayout page_layout, int nested)
{
	print_spaces(nested);
	printf("Page layout: %d\n", page_layout);

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_viewer_preferences(ViewerPreferencesHandle preferences, int nested)
{
	Duplex duplex;
	PrintScaling print_scaling;
	NonFullScreenPageMode page_mode;
	ReadingOrder reading_order;
	BooleanHandle boolean = NULL;
	NameHandle name = NULL;
	IntegerHandle integer = NULL;
	PageRangeHandle page_range = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetHideToolbar(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetHideMenubar(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetHideWindowUI(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetFitWindow(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetDisplayDocTitle(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(ViewerPreferences_GetNonFullScreenPageMode(preferences, &page_mode), proces_nonfullscreen_page_mode(page_mode, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(ViewerPreferences_GetDirection(preferences, &reading_order), proces_reading_order(reading_order, nested + 1));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetViewArea(preferences, &name), process_name(name, nested + 1), NameObject_Release(name));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetViewClip(preferences, &name), process_name(name, nested + 1), NameObject_Release(name));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetPrintArea(preferences, &name), process_name(name, nested + 1), NameObject_Release(name));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetPrintClip(preferences, &name), process_name(name, nested + 1), NameObject_Release(name));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(ViewerPreferences_GetPrintScaling(preferences, &print_scaling), proces_print_scaling(print_scaling, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(ViewerPreferences_GetDuplex(preferences, &duplex), process_duplex(duplex, nested + 1));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetPickTrayByPDFSize(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetPrintPageRange(preferences, &page_range), process_page_range(page_range, nested + 1), PageRange_Release(page_range));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetNumCopies(preferences, &integer), process_integer(integer, nested + 1), IntegerObject_Release(integer));

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_page_range(PageRangeHandle range, int nested)
{
	integer_type i = 0;
	integer_type size = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(PageRange_GetSize(range, &size));

	for (i = 1; i <= size; ++i)
	{
		PageSubRangeHandle sub_range = NULL;
		IntegerHandle first_page = NULL;
		IntegerHandle last_page = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageRange_GetSubrange(range, i, &sub_range));
		RETURN_ERROR_IF_NOT_SUCCESS(PageSubRange_GetFirstPage(sub_range, &first_page));
		RETURN_ERROR_IF_NOT_SUCCESS(PageSubRange_GetLastPage(sub_range, &last_page));

		RETURN_ERROR_IF_NOT_SUCCESS(process_integer(first_page, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(process_integer(last_page, nested + 1));

		RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(first_page));
		RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(last_page));
		RETURN_ERROR_IF_NOT_SUCCESS(PageSubRange_Release(sub_range));
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type proces_nonfullscreen_page_mode(NonFullScreenPageMode page_mode, int nested)
{
	print_spaces(nested);
	printf("Non-FullScreen page Mode: %d\n", page_mode);

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type proces_reading_order(ReadingOrder order, int nested)
{
	print_spaces(nested);
	printf("Reading order: %d\n", order);

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type proces_print_scaling(PrintScaling scaling, int nested)
{
	print_spaces(nested);
	printf("Print scaling: %d\n", scaling);

	return GOTCHANG_PDF_ERROR_SUCCES;
}

error_type process_duplex(Duplex duplex, int nested)
{
	print_spaces(nested);
	printf("Duplex: %d\n", duplex);

	return GOTCHANG_PDF_ERROR_SUCCES;
}