#include "test.h"

error_type process_document(DocumentHandle* document, int nested) {
	CatalogHandle* catalog = NULL;
	DocumentInfoHandle* info = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, &catalog));
	RETURN_ERROR_IF_NOT_SUCCESS(process_catalog(catalog, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_Release(catalog));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Document_GetDocumentInfo(document, &info),
		process_document_info(info, nested + 1),
		DocumentInfo_Release(info));

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_object(ContentObjectHandle* obj, int nested) {
	ContentObjectType type;
	ContentObjectTextHandle* text_object = NULL;
	ContentObjectInlineImageHandle* image_object = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_GetObjectType(obj, &type));
	switch (type) {
		case ContentObjectType_Text:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_FromContentObject(obj, &text_object));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_object_text(text_object, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_Release(text_object));
			break;
		case ContentObjectType_InlineImage:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_ToInlineImage(obj, &image_object));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_object_inline_image(image_object, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_Release(image_object));
			break;
		default:
			print_spaces(nested + 1);
			print_text("Unrecognized content object type\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_object_inline_image(ContentObjectInlineImageHandle* obj, int nested) {
	DictionaryObjectHandle* dictionary = NULL;
	BufferHandle* data = NULL;

	print_spaces(nested);
	print_text("Inline image object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetDictionary(obj, &dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetData(obj, &data));

	RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(data, nested + 1));

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(data));

	print_spaces(nested);
	print_text("Inline image object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_object_text(ContentObjectTextHandle* obj, int nested) {
	size_type i = 0;
	size_type size = 0;

	print_spaces(nested);
	print_text("Text object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationsSize(obj, &size));

	print_spaces(nested + 1);
	unsigned long long converted_size = size;
	print_text("Operations: %llu\n", converted_size);

	for (i = 0; i < size; ++i) {
		ContentOperationHandle* operation = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectText_GetOperationAt(obj, i, &operation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation(operation, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_Release(operation));
	}

	print_spaces(nested);
	print_text("Text object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_operation(ContentOperationHandle* obj, int nested) {
	ContentOperationType type = ContentOperationType_Undefined;
	ContentOperationGenericHandle* generic_operation = NULL;
	ContentOperationTextShowHandle* text_show_operation = NULL;
	ContentOperationTextFontHandle* text_font_operation = NULL;
	ContentOperationTextShowArrayHandle* text_show_array_operation = NULL;
	ContentOperationEndTextHandle* end_text_operation = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_GetOperationType(obj, &type));
	switch (type) {
		case ContentOperationType_Generic:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_FromContentOperation(obj, &generic_operation));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_generic(generic_operation, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_Release(generic_operation));
			break;
		case ContentOperationType_TextShow:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShow_FromContentOperation(obj, &text_show_operation));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_textshow(text_show_operation, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShow_Release(text_show_operation));
			break;
		case ContentOperationType_TextFont:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextFont_FromContentOperation(obj, &text_font_operation));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_textfont(text_font_operation, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextFont_Release(text_font_operation));
			break;
		case ContentOperationType_TextShowArray:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShowArray_FromContentOperation(obj, &text_show_array_operation));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_textshowarray(text_show_array_operation, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShowArray_Release(text_show_array_operation));
			break;
		case ContentOperationType_EndText:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_ToEndText(obj, &end_text_operation));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation_endtext(end_text_operation, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationEndText_Release(end_text_operation));
			break;
		default:
			print_spaces(nested + 1);
			print_text("Unrecognized content operation type\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_operation_generic(ContentOperationGenericHandle* obj, int nested) {
	size_type i = 0;
	size_type size = 0;
	ContentOperatorHandle* oper = NULL;

	print_spaces(nested);
	print_text("Generic content operation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_GetOperator(obj, &oper));
	RETURN_ERROR_IF_NOT_SUCCESS(process_content_operator(oper, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperator_Release(oper));

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_GetOperandsSize(obj, &size));

	print_spaces(nested + 1);
	unsigned long long converted_size = size;
	print_text("Operands: %llu\n", converted_size);

	for (i = 0; i < size; ++i) {
		ObjectHandle* operand = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationGeneric_GetOperandAt(obj, i, &operand));
		RETURN_ERROR_IF_NOT_SUCCESS(process_object(operand, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(operand));
	}

	print_spaces(nested);
	print_text("Generic content operation end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_operation_endtext(ContentOperationEndTextHandle* obj, int nested) {
	print_spaces(nested);
	print_text("Content operation: ET\n");

	// This object has no more properties
	UNUSED(obj);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_operation_textshowarray(ContentOperationTextShowArrayHandle* obj, int nested) {
	ArrayObjectHandle* items_handle = NULL;

	print_spaces(nested);
	print_text("Text show array operation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShowArray_GetValue(obj, &items_handle));
	RETURN_ERROR_IF_NOT_SUCCESS(process_array(items_handle, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Release(items_handle));

	print_spaces(nested);
	print_text("Text show array operation end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_operation_textfont(ContentOperationTextFontHandle* obj, int nested) {
	NameObjectHandle* font_name = NULL;
	RealObjectHandle* font_scale = NULL;

	print_spaces(nested);
	print_text("Text font operation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextFont_GetName(obj, &font_name));
	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextFont_GetScale(obj, &font_scale));
	RETURN_ERROR_IF_NOT_SUCCESS(process_name(font_name, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(process_real(font_scale, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(font_name));
	RETURN_ERROR_IF_NOT_SUCCESS(RealObject_Release(font_scale));

	print_spaces(nested);
	print_text("Text font operation end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_operation_textshow(ContentOperationTextShowHandle* obj, int nested) {
	StringObjectHandle* str = NULL;

	print_spaces(nested);
	print_text("Text show operation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperationTextShow_GetValue(obj, &str));
	RETURN_ERROR_IF_NOT_SUCCESS(process_string(str, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Release(str));

	print_spaces(nested);
	print_text("Text show operation end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_operator(ContentOperatorHandle* obj, int nested) {
	ContentOperatorType type;
	BufferHandle* buffer = NULL;

	print_spaces(nested);
	print_text("Content operator begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperator_GetOperatorType(obj, &type));

	print_spaces(nested + 1);
	print_text("Type: %d\n", type);

	RETURN_ERROR_IF_NOT_SUCCESS(ContentOperator_GetValue(obj, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	print_text("Content operator end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_content_instruction(ContentInstructionHandle* obj, int nested) {
	ContentInstructionType type;
	ContentObjectHandle* object_handle = NULL;
	ContentOperationHandle* operation_handle = NULL;

	print_spaces(nested);
	print_text("Content instruction\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_GetInstructionType(obj, &type));
	switch (type) {
		case ContentInstructionType_Object:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_FromInstruction(obj, &object_handle));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_object(object_handle, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_Release(object_handle));
			break;
		case ContentInstructionType_Operation:
			RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_ToOperation(obj, &operation_handle));
			RETURN_ERROR_IF_NOT_SUCCESS(process_content_operation(operation_handle, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(ContentOperation_Release(operation_handle));
			break;
		default:
			print_spaces(nested + 1);
			print_text("Unrecognized content instruction type\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	print_spaces(nested);
	print_text("Content instruction end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_page_contents(PageContentsHandle* obj, int nested) {
	size_type i = 0;
	size_type size = 0;

	print_spaces(nested);
	print_text("Contents begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(PageContents_GetInstructionsSize(obj, &size));

	print_spaces(nested + 1);
	unsigned long long converted_size = size;
	print_text("Size: %llu\n", converted_size);

	for (i = 0; i < size; ++i) {
		ContentInstructionHandle* instruction = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageContents_GetInstructionAt(obj, i, &instruction));
		RETURN_ERROR_IF_NOT_SUCCESS(process_content_instruction(instruction, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_Release(instruction));
	}

	print_spaces(nested);
	print_text("Contents end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_font_map(FontMapHandle* obj, int nested) {
	print_spaces(nested);
	print_text("Font map begin\n");

	// TODO font map properties
	UNUSED(obj);

	print_spaces(nested);
	print_text("Font map end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_resource_dictionary(ResourceDictionaryHandle* obj, int nested) {
	FontMapHandle* font_map = NULL;

	print_spaces(nested);
	print_text("Resource dictionary begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ResourceDictionary_GetFontMap(obj, &font_map),
		process_font_map(font_map, nested + 1),
		FontMap_Release(font_map));

	print_spaces(nested);
	print_text("Resource dictionary end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_rectangle(RectangleHandle* obj, int nested) {
	bigint_type lower_left_x = 0;
	bigint_type lower_left_y = 0;
	bigint_type upper_right_x = 0;
	bigint_type upper_right_y = 0;

	print_spaces(nested);
	print_text("Rectangle begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Rectangle_GetLowerLeftX(obj, &lower_left_x));
	RETURN_ERROR_IF_NOT_SUCCESS(Rectangle_GetLowerLeftY(obj, &lower_left_y));
	RETURN_ERROR_IF_NOT_SUCCESS(Rectangle_GetUpperRightX(obj, &upper_right_x));
	RETURN_ERROR_IF_NOT_SUCCESS(Rectangle_GetUpperRightY(obj, &upper_right_y));

	print_spaces(nested + 1); print_text("Lower left X: %lld\n", lower_left_x);
	print_spaces(nested + 1); print_text("Lower left Y: %lld\n", lower_left_y);
	print_spaces(nested + 1); print_text("Upper right X: %lld\n", upper_right_x);
	print_spaces(nested + 1); print_text("Upper right Y: %lld\n", upper_right_y);

	print_spaces(nested);
	print_text("Rectangle end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_link_annotation(LinkAnnotationHandle* obj, int nested) {
	DestinationHandle* destination = NULL;

	print_spaces(nested);
	print_text("Link annotation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(LinkAnnotation_GetDestination(obj, &destination),
		process_destination(destination, nested + 1),
		Destination_Release(destination));

	print_spaces(nested);
	print_text("Link annotation end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_annotation(AnnotationHandle* obj, int nested) {
	AnnotationType type;
	LinkAnnotationHandle* link_annotation = NULL;

	print_spaces(nested);
	print_text("Annotation begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Annotation_GetAnnotationType(obj, &type));

	switch (type) {
		case AnnotationType_Link:
			RETURN_ERROR_IF_NOT_SUCCESS(LinkAnnotation_FromBaseAnnotation(obj, &link_annotation));
			RETURN_ERROR_IF_NOT_SUCCESS(process_link_annotation(link_annotation, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(LinkAnnotation_Release(link_annotation));
			break;

		case AnnotationType_Text:
		case AnnotationType_FreeText:
		case AnnotationType_Line:
		case AnnotationType_Square:
		case AnnotationType_Circle:
		case AnnotationType_Polygon:
		case AnnotationType_PolyLine:
		case AnnotationType_Highlight:
		case AnnotationType_Underline:
		case AnnotationType_Squiggly:
		case AnnotationType_StrikeOut:
		case AnnotationType_RubberStamp:
		case AnnotationType_Caret:
		case AnnotationType_Ink:
		case AnnotationType_Popup:
		case AnnotationType_FileAttachment:
		case AnnotationType_Sound:
		case AnnotationType_Movie:
		case AnnotationType_Widget:
		case AnnotationType_Screen:
		case AnnotationType_PrinterMark:
		case AnnotationType_TrapNetwork:
		case AnnotationType_Watermark:
		case AnnotationType_TripleD:
		case AnnotationType_Redaction:
			print_spaces(nested + 1);
			print_text("Type: %d\n", type);
			break;

		default:
			print_spaces(nested + 1);
			print_text("Unrecognized annotation type\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	print_spaces(nested);
	print_text("Annotation end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_page_annotations(PageAnnotationsHandle* obj, int nested) {
	size_type annotation_count = 0;
	size_type i = 0;

	print_spaces(nested);
	print_text("Page annotations begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(PageAnnotations_GetSize(obj, &annotation_count));

	for (i = 0; i < annotation_count; ++i) {
		AnnotationHandle* annotation = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(PageAnnotations_At(obj, i, &annotation));
		RETURN_ERROR_IF_NOT_SUCCESS(process_annotation(annotation, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Annotation_Release(annotation));
	}

	print_spaces(nested);
	print_text("Page annotations end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_page(PageObjectHandle* obj, int nested) {
	PageContentsHandle* contents = NULL;
	RectangleHandle* media_box = NULL;
	PageAnnotationsHandle* annotations = NULL;
	ResourceDictionaryHandle* page_resources = NULL;

	print_spaces(nested);
	print_text("Page begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageObject_GetContents(obj, &contents),
		process_page_contents(contents, nested + 1),
		PageContents_Release(contents));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageObject_GetMediaBox(obj, &media_box),
		process_rectangle(media_box, nested + 1),
		Rectangle_Release(media_box));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageObject_GetAnnotations(obj, &annotations),
		process_page_annotations(annotations, nested + 1),
		PageAnnotations_Release(annotations));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageObject_GetResources(obj, &page_resources),
		process_resource_dictionary(page_resources, nested + 1),
		ResourceDictionary_Release(page_resources));

	print_spaces(nested);
	print_text("Page end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_extensions(DeveloperExtensionsHandle* extensions, int nested) {
	boolean_type boolean = VANILLAPDF_RV_FALSE;
	DeveloperExtensionsIteratorHandle* iterator = NULL;

	print_spaces(nested);
	print_text("Developer extensions begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensions_GetIterator(extensions, &iterator));
	while (VANILLAPDF_ERROR_SUCCESS == DeveloperExtensionsIterator_IsValid(iterator, &boolean)
		&& VANILLAPDF_RV_TRUE == boolean) {
		NameObjectHandle* key = NULL;
		DeveloperExtensionHandle* value = NULL;

		print_spaces(nested);
		print_text("Pair:\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_GetKey(iterator, &key));
		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_GetValue(iterator, &value));

		RETURN_ERROR_IF_NOT_SUCCESS(process_name(key, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(process_extension(value, nested + 1));

		RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(key));
		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtension_Release(value));

		print_spaces(nested);
		print_text("EndPair\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_Next(iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtensionsIterator_Release(iterator));

	print_spaces(nested);
	print_text("Developer extensions End\n");
	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_extension(DeveloperExtensionHandle* extension, int nested) {
	IntegerObjectHandle* level = NULL;
	PDFVersion base_version = PDFVersion_Undefined;

	print_spaces(nested);
	print_text("Developer extensions begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtension_GetBaseVersion(extension, &base_version));
	RETURN_ERROR_IF_NOT_SUCCESS(DeveloperExtension_GetExtensionLevel(extension, &level));

	RETURN_ERROR_IF_NOT_SUCCESS(process_version(base_version, nested + 1));

	print_spaces(nested + 1);
	print_text("Extensions level begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(level, nested + 2));

	print_spaces(nested + 1);
	print_text("Extensions level end\n");

	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(level));

	print_spaces(nested);
	print_text("Developer extensions end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_named_destinations(NamedDestinationsHandle* obj, int nested) {
	print_spaces(nested);
	print_text("Named destinations begin\n");

	// TODO Name destinations needs iterator
	UNUSED(obj);

	print_spaces(nested);
	print_text("Named destinations end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_destination(DestinationHandle* obj, int nested) {
	ObjectHandle* page = NULL;

	print_spaces(nested);
	print_text("Destination begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Destination_GetPageNumber(obj, &page));
	RETURN_ERROR_IF_NOT_SUCCESS(process_object(page, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(page));

	print_spaces(nested);
	print_text("Destination end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_catalog(CatalogHandle* catalog, int nested) {
	size_type i = 0;
	size_type size = 0;
	PageTreeHandle* pages = NULL;
	DeveloperExtensionsHandle* extensions = NULL;
	PageLabelsHandle* page_labels = NULL;
	ViewerPreferencesHandle* viewer_preferences = NULL;
	OutlineHandle* outlines = NULL;
	PDFVersion version = PDFVersion_Undefined;
	PageLayout page_layout = PageLayout_Undefined;
	NamedDestinationsHandle* named_destinations = NULL;
	InteractiveFormHandle* interactive_form = NULL;

	print_spaces(nested);
	print_text("Document catalog begin\n");

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

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetDestinations(catalog, &named_destinations),
		process_named_destinations(named_destinations, nested + 1),
		NamedDestinations_Release(named_destinations));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(Catalog_GetAcroForm(catalog, &interactive_form),
		process_interactive_form(interactive_form, nested + 1),
		InteractiveForm_Release(interactive_form));

	for (i = 1; i <= size; ++i) {
		PageObjectHandle* page = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(pages, i, &page));
		RETURN_ERROR_IF_NOT_SUCCESS(process_page(page, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(pages));

	print_spaces(nested);
	print_text("Document catalog end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_trapped(DocumentTrappedType trapped, int nested) {
	print_spaces(nested);
	print_text("Trapped: %d\n", trapped);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_document_info(DocumentInfoHandle* obj, int nested) {
	StringObjectHandle* title = NULL;
	StringObjectHandle* author = NULL;
	StringObjectHandle* subject = NULL;
	StringObjectHandle* keywords = NULL;
	StringObjectHandle* creator = NULL;
	StringObjectHandle* producer = NULL;
	DateHandle* creation_date = NULL;
	DateHandle* modification_date = NULL;
	DocumentTrappedType trapped;

	print_spaces(nested);
	print_text("Document info begin\n");

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

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(DocumentInfo_GetTrapped(obj, &trapped),
		process_trapped(trapped, nested + 1));

	print_spaces(nested);
	print_text("Document info end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

//! [Print date]
error_type process_date(DateHandle* obj, int nested) {
	integer_type year = 0;
	integer_type month = 0;
	integer_type day = 0;
	integer_type hour = 0;
	integer_type minute = 0;
	integer_type second = 0;
	TimezoneType timezone;
	integer_type hour_offset = 0;
	integer_type minute_offset = 0;

	print_spaces(nested);
	print_text("Date begin\n");

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
	if (timezone == TimezoneType_UTC) {
		print_text("%04d-%02d-%02d %02d:%02d:%02dZ\n",
			year, month, day,
			hour, minute, second);
	} else {
		char timezone_character;
		if (timezone == TimezoneType_Later) {
			timezone_character = '+';
		} else if (timezone == TimezoneType_Earlier) {
			timezone_character = '-';
		} else {
			print_text("Timezone is neither UTC, Earlier nor later\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
		}

		print_text("%04d-%02d-%02d %02d:%02d:%02d %c%02d:%02d\n",
			year, month, day,
			hour, minute, second,
			timezone_character, hour_offset, minute_offset);
	}

	print_spaces(nested);
	print_text("Date end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
//! [Print date]

error_type process_outline(OutlineHandle* outline, int nested) {
	OutlineItemHandle* first = NULL;
	OutlineItemHandle* last = NULL;
	IntegerObjectHandle* count = NULL;

	print_spaces(nested);
	print_text("Document outline begin\n");

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
	print_text("Document outline end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_outline_item(OutlineItemHandle* outline, int nested) {
	OutlineBaseHandle* parent = NULL;
	OutlineItemHandle* first = NULL;
	OutlineItemHandle* last = NULL;
	OutlineItemHandle* next = NULL;
	OutlineItemHandle* prev = NULL;
	IntegerObjectHandle* count = NULL;
	StringObjectHandle* title = NULL;
	OutlineItemColorHandle* color = NULL;
	OutlineItemFlagsHandle* flags = NULL;

	print_spaces(nested);
	print_text("Document outline begin\n");

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

	// Prevent endless cycles and test previous without more processing
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(OutlineItem_GetPrev(outline, &prev),
		// This is a little trick, it just passes trough the macro
		// We intentionally do not want to process this item
		VANILLAPDF_ERROR_SUCCESS,
		OutlineItem_Release(prev));

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
	print_text("Document outline end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_outline_base(OutlineBaseHandle* outline, int nested) {
	OutlineType type;

	print_spaces(nested);
	print_text("Outline base begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(OutlineBase_GetOutlineType(outline, &type));

	print_spaces(nested + 1);
	print_text("Type: %d\n", type);

	print_spaces(nested);
	print_text("Outline base end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_outline_item_color(OutlineItemColorHandle* obj, int nested) {
	IntegerObjectHandle* red = NULL;
	IntegerObjectHandle* green = NULL;
	IntegerObjectHandle* blue = NULL;

	print_spaces(nested);
	print_text("Outline item color begin\n");

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
	print_text("Outline item color end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_outline_item_flags(OutlineItemFlagsHandle* obj, int nested) {
	boolean_type is_italic = VANILLAPDF_RV_FALSE;
	boolean_type is_bold = VANILLAPDF_RV_FALSE;

	print_spaces(nested);
	print_text("Outline item flags begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItemFlags_IsItalic(obj, &is_italic));
	RETURN_ERROR_IF_NOT_SUCCESS(OutlineItemFlags_IsBold(obj, &is_bold));

	print_spaces(nested + 1);
	print_text("Italic: %d\n", is_italic);

	print_spaces(nested + 1);
	print_text("Bold: %d\n", is_bold);

	print_spaces(nested);
	print_text("Outline item flags end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_page_labels(PageLabelsHandle* labels, size_type size, int nested) {
	size_type i = 0;

	print_spaces(nested);
	print_text("Page labels begin\n");

	for (i = 1; i <= size; ++i) {
		boolean_type contains = VANILLAPDF_RV_FALSE;
		PageLabelHandle* label = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageLabels_Contains(labels, i, &contains));
		if (VANILLAPDF_RV_TRUE != contains) {
			continue;
		}

		RETURN_ERROR_IF_NOT_SUCCESS(PageLabels_At(labels, i, &label));
		RETURN_ERROR_IF_NOT_SUCCESS(process_page_label(label, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(PageLabel_Release(label));
	}

	print_spaces(nested);
	print_text("Page labels end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_page_label(PageLabelHandle* label, int nested) {
	StringObjectHandle* p = NULL;
	IntegerObjectHandle* st = NULL;
	NumberingStyle s;

	print_spaces(nested);
	print_text("Page label begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageLabel_Prefix(label, &p), process_string(p, nested + 1), StringObject_Release(p));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(PageLabel_Start(label, &st), process_integer(st, nested + 1), IntegerObject_Release(st));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL(PageLabel_Style(label, &s), proces_numbering_style(s, nested + 1));

	print_spaces(nested);
	print_text("Page label end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type proces_numbering_style(NumberingStyle style, int nested) {
	print_spaces(nested + 1);
	print_text("Numbering Style: %d\n", style);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_page_layout(PageLayout page_layout, int nested) {
	print_spaces(nested);
	print_text("Page layout: %d\n", page_layout);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_viewer_preferences(ViewerPreferencesHandle* preferences, int nested) {
	Duplex duplex;
	PrintScaling print_scaling;
	NonFullScreenPageMode page_mode;
	ReadingOrder reading_order;
	BooleanObjectHandle* boolean = NULL;
	NameObjectHandle* name = NULL;
	IntegerObjectHandle* integer = NULL;
	PageRangeHandle* page_range = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetHideToolbar(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetHideMenubar(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetHideWindowUI(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetFitWindow(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(ViewerPreferences_GetCenterWindow(preferences, &boolean), process_boolean(boolean, nested + 1), BooleanObject_Release(boolean));
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

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_page_range(PageRangeHandle* range, int nested) {
	size_type i = 0;
	size_type size = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(PageRange_GetSize(range, &size));

	for (i = 1; i <= size; ++i) {
		PageSubRangeHandle* sub_range = NULL;
		IntegerObjectHandle* first_page = NULL;
		IntegerObjectHandle* last_page = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageRange_GetSubrange(range, i, &sub_range));
		RETURN_ERROR_IF_NOT_SUCCESS(PageSubRange_GetFirstPage(sub_range, &first_page));
		RETURN_ERROR_IF_NOT_SUCCESS(PageSubRange_GetLastPage(sub_range, &last_page));

		RETURN_ERROR_IF_NOT_SUCCESS(process_integer(first_page, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(process_integer(last_page, nested + 1));

		RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(first_page));
		RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(last_page));
		RETURN_ERROR_IF_NOT_SUCCESS(PageSubRange_Release(sub_range));
	}

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type proces_nonfullscreen_page_mode(NonFullScreenPageMode page_mode, int nested) {
	print_spaces(nested);
	print_text("Non-FullScreen page Mode: %d\n", page_mode);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type proces_reading_order(ReadingOrder order, int nested) {
	print_spaces(nested);
	print_text("Reading order: %d\n", order);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type proces_print_scaling(PrintScaling scaling, int nested) {
	print_spaces(nested);
	print_text("Print scaling: %d\n", scaling);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_duplex(Duplex duplex, int nested) {
	print_spaces(nested);
	print_text("Duplex: %d\n", duplex);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_document_save(
	DocumentHandle* document,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested) {

	FileHandle* save_file = NULL;
	FileHandle* load_file = NULL;

	InputOutputStreamHandle* input_output_stream = NULL;
	boolean_type is_encrypted = VANILLAPDF_RV_FALSE;

	print_spaces(nested);
	print_text("Process document save begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_CreateFromMemory(&input_output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(File_CreateStream(input_output_stream, "UNUSED", &save_file));

	//RETURN_ERROR_IF_NOT_SUCCESS(Document_Save(document, "C:\\temp\\output.pdf"));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_SaveFile(document, save_file));

	// Check the file consistency
	RETURN_ERROR_IF_NOT_SUCCESS(File_OpenStream(input_output_stream, "UNUSED", &load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_IsEncrypted(load_file, &is_encrypted));

	if (is_encrypted == VANILLAPDF_RV_TRUE) {
		// No password entered
		if (password == NULL && cert_path == NULL) {
			// Opening the file with default password
		}

		if (password != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(load_file, password));
		}

		if (cert_path != NULL) {
			PKCS12KeyHandle* pkcs12_key = NULL;
			EncryptionKeyHandle* encryption_key = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(cert_path, cert_password, &pkcs12_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(pkcs12_key, &encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(load_file, encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));
		}
	}
	else {
		// Password for un-encrypted file
		if (password != NULL || cert_path != NULL) {
			return VANILLAPDF_TEST_ERROR_INVALID_PASSWORD;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(load_file, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(load_file));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(save_file));
	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_Release(input_output_stream));

	print_spaces(nested);
	print_text("Process document save end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_document_save_incremental(
	DocumentHandle* document,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested) {

	FileHandle* save_file = NULL;
	FileHandle* load_file = NULL;

	InputOutputStreamHandle* input_output_stream = NULL;
	boolean_type is_encrypted = VANILLAPDF_RV_FALSE;

	print_spaces(nested);
	print_text("Process document save incremental begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_CreateFromMemory(&input_output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(File_CreateStream(input_output_stream, "UNUSED", &save_file));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_SaveIncrementalFile(document, save_file));

	// Check the file consistency
	RETURN_ERROR_IF_NOT_SUCCESS(File_OpenStream(input_output_stream, "UNUSED", &load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_IsEncrypted(load_file, &is_encrypted));

	if (is_encrypted == VANILLAPDF_RV_TRUE) {
		// No password entered
		if (password == NULL && cert_path == NULL) {
			// Opening the file with default password
		}

		if (password != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(load_file, password));
		}

		if (cert_path != NULL) {
			PKCS12KeyHandle* pkcs12_key = NULL;
			EncryptionKeyHandle* encryption_key = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(cert_path, cert_password, &pkcs12_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(pkcs12_key, &encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(load_file, encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));
		}
	}
	else {
		// Password for un-encrypted file
		if (password != NULL || cert_path != NULL) {
			return VANILLAPDF_TEST_ERROR_INVALID_PASSWORD;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(load_file, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(load_file));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(save_file));
	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_Release(input_output_stream));

	print_spaces(nested);
	print_text("Process document save incremental end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_document_merge(
	DocumentHandle* document,
	string_type merge_file,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested) {

	FileHandle* save_file = NULL;
	FileHandle* load_file = NULL;
	DocumentHandle* other_document = NULL;
	InputOutputStreamHandle* input_output_stream = NULL;
	boolean_type is_encrypted = VANILLAPDF_RV_FALSE;

	if (merge_file == NULL) {
		return VANILLAPDF_TEST_ERROR_SUCCESS;
	}

	print_spaces(nested);
	print_text("Process document merge begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_CreateFromMemory(&input_output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(File_CreateStream(input_output_stream, "UNUSED", &save_file));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Open(merge_file, &other_document));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_AppendDocument(document, other_document));
	RETURN_ERROR_IF_NOT_SUCCESS(Document_Release(other_document));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_SaveFile(document, save_file));

	// Check the file consistency
	RETURN_ERROR_IF_NOT_SUCCESS(File_OpenStream(input_output_stream, "UNUSED", &load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_IsEncrypted(load_file, &is_encrypted));

	if (is_encrypted == VANILLAPDF_RV_TRUE) {
		// No password entered
		if (password == NULL && cert_path == NULL) {
			// Opening the file with default password
		}

		if (password != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(load_file, password));
		}

		if (cert_path != NULL) {
			PKCS12KeyHandle* pkcs12_key = NULL;
			EncryptionKeyHandle* encryption_key = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(cert_path, cert_password, &pkcs12_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(pkcs12_key, &encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(load_file, encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(pkcs12_key));
		}
	}
	else {
		// Password for un-encrypted file
		if (password != NULL || cert_path != NULL) {
			return VANILLAPDF_TEST_ERROR_INVALID_PASSWORD;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(load_file, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(load_file));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(save_file));
	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_Release(input_output_stream));

	print_spaces(nested);
	print_text("Process document merge end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_document_sign(
	DocumentHandle* document,
	string_type key_file,
	string_type key_password,
	string_type password,
	string_type cert_path,
	string_type cert_password,
	int nested) {

	FileHandle* save_file = NULL;
	FileHandle* load_file = NULL;
	InputOutputStreamHandle* input_output_stream = NULL;

	PKCS12KeyHandle* signature_pkcs12_key = NULL;
	SigningKeyHandle* signing_key = NULL;
	DateHandle* signing_time = NULL;
	DocumentSignatureSettingsHandle* signature_settings = NULL;
	boolean_type is_encrypted = VANILLAPDF_RV_FALSE;

	print_spaces(nested);
	print_text("Process document sign begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_CreateFromMemory(&input_output_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(File_CreateStream(input_output_stream, "UNUSED", &save_file));
	//RETURN_ERROR_IF_NOT_SUCCESS(File_Create("C:\\temp\\output.pdf", &save_file));

	RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(key_file, key_password, &signature_pkcs12_key));
	RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToSigningKey(signature_pkcs12_key, &signing_key));

	RETURN_ERROR_IF_NOT_SUCCESS(Date_CreateCurrent(&signing_time));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Create(&signature_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256));
	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time));

	RETURN_ERROR_IF_NOT_SUCCESS(Document_Sign(document, save_file, signature_settings));

	// Check the file consistency
	RETURN_ERROR_IF_NOT_SUCCESS(File_OpenStream(input_output_stream, "UNUSED", &load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(load_file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_IsEncrypted(load_file, &is_encrypted));

	if (is_encrypted == VANILLAPDF_RV_TRUE) {
		// No password entered
		if (password == NULL && cert_path == NULL) {
			// Opening the file with default password
		}

		if (password != NULL) {
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionPassword(load_file, password));
		}

		if (cert_path != NULL) {
			PKCS12KeyHandle* encryption_pkcs12_key = NULL;
			EncryptionKeyHandle* encryption_key = NULL;

			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_CreateFromFile(cert_path, cert_password, &encryption_pkcs12_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_ToEncryptionKey(encryption_pkcs12_key, &encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(File_SetEncryptionKey(load_file, encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(EncryptionKey_Release(encryption_key));
			RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(encryption_pkcs12_key));
		}
	}
	else {
		// Password for un-encrypted file
		if (password != NULL || cert_path != NULL) {
			return VANILLAPDF_TEST_ERROR_INVALID_PASSWORD;
		}
	}

	RETURN_ERROR_IF_NOT_SUCCESS(process_file(load_file, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(load_file));

	RETURN_ERROR_IF_NOT_SUCCESS(DocumentSignatureSettings_Release(signature_settings));
	RETURN_ERROR_IF_NOT_SUCCESS(Date_Release(signing_time));

	RETURN_ERROR_IF_NOT_SUCCESS(SigningKey_Release(signing_key));
	RETURN_ERROR_IF_NOT_SUCCESS(PKCS12Key_Release(signature_pkcs12_key));

	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(save_file));
	RETURN_ERROR_IF_NOT_SUCCESS(InputOutputStream_Release(input_output_stream));

	print_spaces(nested);
	print_text("Process document sign end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_interactive_form(InteractiveFormHandle* obj, int nested) {
	FieldCollectionHandle* fields = NULL;

	print_spaces(nested);
	print_text("Interactive form begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(InteractiveForm_GetFields(obj, &fields),
		process_field_collection(fields, nested + 1),
		FieldCollection_Release(fields));

	print_spaces(nested);
	print_text("Interactive form end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_field_collection(FieldCollectionHandle* obj, int nested) {
	size_type i = 0;
	size_type size = 0;

	print_spaces(nested);
	print_text("Field collection begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(FieldCollection_GetSize(obj, &size));

	print_spaces(nested);

	unsigned long long converted_size = size;
	print_text("Size: %llu\n", converted_size);

	for (i = 0; i < size; ++i) {
		FieldHandle* field = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(FieldCollection_At(obj, i, &field));
		RETURN_ERROR_IF_NOT_SUCCESS(process_field(field, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Field_Release(field));
	}

	print_spaces(nested);
	print_text("Field collection end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_field(FieldHandle* obj, int nested) {
	FieldType type;
	SignatureFieldHandle* signature_field = NULL;

	print_spaces(nested);
	print_text("Field begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Field_GetType(obj, &type));

	switch (type) {
		case FieldType_Signature:
			RETURN_ERROR_IF_NOT_SUCCESS(Field_ToSignature(obj, &signature_field));
			RETURN_ERROR_IF_NOT_SUCCESS(process_signature_field(signature_field, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(SignatureField_Release(signature_field));
			break;

		case FieldType_NonTerminal:
		case FieldType_Button:
		case FieldType_Text:
		case FieldType_Choice:
			print_spaces(nested + 1);
			print_text("Type: %d\n", type);
			break;

		default:
			print_spaces(nested + 1);
			print_text("Unrecognized field type\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	print_spaces(nested);
	print_text("Field end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_signature_field(SignatureFieldHandle* obj, int nested) {
	DigitalSignatureHandle* digital_signature = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(SignatureField_GetValue(obj, &digital_signature));
	RETURN_ERROR_IF_NOT_SUCCESS(process_digital_signature(digital_signature, nested));
	RETURN_ERROR_IF_NOT_SUCCESS(DigitalSignature_Release(digital_signature));

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_digital_signature(DigitalSignatureHandle* obj, int nested) {
	StringObjectHandle* contact_info = NULL;
	StringObjectHandle* name = NULL;
	StringObjectHandle* reason = NULL;
	StringObjectHandle* location = NULL;
	IntegerObjectHandle* revision = NULL;
	StringObjectHandle* certificate = NULL;
	ByteRangeCollectionHandle* byte_range = NULL;
	HexadecimalStringObjectHandle* contents = NULL;
	DateHandle* signing_time = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetContactInfo(obj, &contact_info),
	process_string(contact_info, nested + 1),
	StringObject_Release(contact_info));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetName(obj, &name),
	process_string(name, nested + 1),
	StringObject_Release(name));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetLocation(obj, &location),
	process_string(location, nested + 1),
	StringObject_Release(location));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetReason(obj, &reason),
	process_string(reason, nested + 1),
	StringObject_Release(reason));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetRevision(obj, &revision),
	process_integer(revision, nested + 1),
	IntegerObject_Release(revision));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetCertificate(obj, &certificate),
	process_string(certificate, nested + 1),
	StringObject_Release(certificate));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetContents(obj, &contents),
	process_hex_string(contents, nested + 1),
	HexadecimalStringObject_Release(contents));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetSigningTime(obj, &signing_time),
	process_date(signing_time, nested + 1),
	Date_Release(signing_time));

	RETURN_ERROR_IF_NOT_SUCCESS_OPTIONAL_RELEASE(DigitalSignature_GetByteRange(obj, &byte_range),
	process_byte_range_collection(byte_range, nested + 1),
	ByteRangeCollection_Release(byte_range));

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_byte_range_collection(ByteRangeCollectionHandle* obj, int nested) {
	size_type i = 0;
	size_type size = 0;

	print_spaces(nested);
	print_text("Byte range collection begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ByteRangeCollection_GetSize(obj, &size));

	print_spaces(nested);

	unsigned long long converted_size = size;
	print_text("Size: %llu\n", converted_size);

	for (i = 0; i < size; ++i) {
		ByteRangeHandle* range = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(ByteRangeCollection_GetValue(obj, i, &range));
		RETURN_ERROR_IF_NOT_SUCCESS(process_byte_range(range, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(ByteRange_Release(range));
	}

	print_spaces(nested);
	print_text("Byte range collection end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_byte_range(ByteRangeHandle* obj, int nested) {
	IntegerObjectHandle* offset = NULL;
	IntegerObjectHandle* length = NULL;

	print_spaces(nested);
	print_text("Byte range begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ByteRange_GetOffset(obj, &offset));
	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(offset, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(offset));

	RETURN_ERROR_IF_NOT_SUCCESS(ByteRange_GetLength(obj, &length));
	RETURN_ERROR_IF_NOT_SUCCESS(process_integer(length, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(length));

	print_spaces(nested);
	print_text("Byte range end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
