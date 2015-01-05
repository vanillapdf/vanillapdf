#include "gotchangpdf/gotchangpdf.h"

#include <stdio.h>
#include <assert.h>

#define RETURN_ERROR_IF_NOT_SUCCESS(var) do { int __result__ = (var);  if (GOTCHANG_PDF_ERROR_SUCCES != __result__) return __result__; } while(0)

int process(ObjectHandle obj, int nested);

void print_spaces(int nested)
{
	int i;
	for (i = 0; i < nested; ++i)
		printf("  ");
}

int process_page(PageObjectHandle obj, int nested)
{
	print_spaces(nested);
	printf("Page begin\n");

	print_spaces(nested);
	printf("Page end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_indirect(IndirectHandle indirect, int nested)
{
	offset_type offset = 0;
	ObjectHandle child = NULL;

	print_spaces(nested);
	printf("Indirect object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(IndirectObject_GetOffset(indirect, &offset));
	print_spaces(nested + 1);
	printf("Offset: %lld\n\n", offset);
	RETURN_ERROR_IF_NOT_SUCCESS(IndirectObject_GetObject(indirect, &child));
	RETURN_ERROR_IF_NOT_SUCCESS(process(child, 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(child));

	print_spaces(nested);
	printf("Indirect object end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_name(NameHandle name, int nested)
{
	string_type str;

	print_spaces(nested);
	printf("Name object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Value(name, &str));

	print_spaces(nested + 1);
	printf("Value: %s\n", str);

	print_spaces(nested);
	printf("Name object end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process(ObjectHandle obj, int nested)
{
	int i, size, val, boolean;
	IndirectHandle indirect;
	IndirectReferenceHandle indirect_reference;
	ArrayHandle arr;
	IntegerHandle integer;
	NameHandle name;
	ObjectHandle child, dictionary_value;
	DictionaryHandle dictionary;
	DictionaryIteratorHandle iterator;
	NameHandle dictionary_key;
	enum ObjectType type;

	RETURN_ERROR_IF_NOT_SUCCESS(Object_Type(obj, &type));

	if (nested > 2)
		return GOTCHANG_PDF_ERROR_SUCCES;

	switch (type)
	{
	case Unknown:
		print_spaces(nested);
		printf("UNRECOGNIZED OBJECT TYPE!\n");
		return GOTCHANG_PDF_ERROR_GENERAL;
	case Array:
		print_spaces(nested);
		printf("Array begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToArray(obj, &arr));
		RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Size(arr, &size));

		print_spaces(nested + 1);
		printf("Size: %d\n\n", size);
		for (i = 0; i < size; ++i)
		{
			RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_At(arr, i, &child));
			RETURN_ERROR_IF_NOT_SUCCESS(process(child, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(child));
		}

		//RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Release(arr));

		print_spaces(nested);
		printf("Array end\n");

		break;
	case Boolean:
		print_spaces(nested);
		printf("Boolean object begin\n");

		print_spaces(nested);
		printf("Boolean object end\n");
		break;
	case Dictionary:
		print_spaces(nested);
		printf("Dictionary begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToDictionary(obj, &dictionary));

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Iterator(dictionary, &iterator));
		while (GOTCHANG_PDF_ERROR_SUCCES == DictionaryObjectIterator_IsValid(iterator, dictionary, &boolean)
			&& GOTCHANG_PDF_RV_TRUE == boolean)
		{
			print_spaces(nested);
			printf("Pair:\n");

			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_GetKey(iterator, &dictionary_key));
			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_GetValue(iterator, &dictionary_value));

			RETURN_ERROR_IF_NOT_SUCCESS(process_name(dictionary_key, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(process(dictionary_value, nested + 1));

			RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(dictionary_key));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(dictionary_value));

			print_spaces(nested);
			printf("EndPair\n");

			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_Next(iterator));
		}

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_Release(iterator));
		//RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));

		print_spaces(nested);
		printf("Dictionary End\n");

		break;
	case Function:
		print_spaces(nested);
		printf("Function object begin\n");

		print_spaces(nested);
		printf("Function object end\n");
		break;
	case Integer:
		print_spaces(nested);
		printf("Integer object begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToInteger(obj, &integer));
		RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Value(integer, &val));
		print_spaces(nested + 1);
		printf("Value: %d\n", val);

		print_spaces(nested);
		printf("Integer object end\n");
		break;
	case Name:
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToName(obj, &name));
		RETURN_ERROR_IF_NOT_SUCCESS(process_name(name, nested));
		break;
		/*
	case NameTree:
		print_spaces(nested);
		printf("Name tree object begin\n");

		print_spaces(nested);
		printf("Name tree object end\n");
		break;
		*/
	case Null:
		print_spaces(nested);
		printf("Null object begin\n");

		printf("Value: null\n");

		print_spaces(nested);
		printf("Null object end\n");
		break;
		/*
	case ObjectStream:
		print_spaces(nested);
		printf("Object stream begin\n");

		print_spaces(nested);
		printf("Object stream end\n");
		break;
		*/
	case Real:
		print_spaces(nested);
		printf("Real object begin\n");

		print_spaces(nested);
		printf("Real object end\n");
		break;
	case Stream:
		print_spaces(nested);
		printf("Stream object begin\n");

		print_spaces(nested);
		printf("Stream object end\n");
		break;
	case HexadecimalString:
		print_spaces(nested);
		printf("Hexadecimal string begin\n");

		print_spaces(nested);
		printf("Hexadecimal string end\n");
		break;
	case LiteralString:
		print_spaces(nested);
		printf("Literal string begin\n");

		print_spaces(nested);
		printf("Literal string end\n");
		break;
	case IndirectReference:
		print_spaces(nested);
		printf("Indirect reference begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToIndirectReference(obj, &indirect_reference));

		RETURN_ERROR_IF_NOT_SUCCESS(IndirectReference_GetReferencedObject(indirect_reference, &indirect));
		RETURN_ERROR_IF_NOT_SUCCESS(process_indirect(indirect, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(IndirectObject_Release(indirect));

		print_spaces(nested);
		printf("Indirect reference end\n");
		break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int main(int argc, char *argv[])
{
	int i, size;
	FileHandle file = NULL;
	XrefHandle xref = NULL;
	CatalogHandle catalog;
	PageTreeHandle pages;

	if (argc != 2)
		return GOTCHANG_PDF_ERROR_GENERAL;

	RETURN_ERROR_IF_NOT_SUCCESS(File_Create(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Xref(file, &xref));
	RETURN_ERROR_IF_NOT_SUCCESS(Xref_Size(xref, &size));

	for (i = 1; i < size; ++i)
	{
		IndirectHandle indirect = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(File_GetIndirectObject(file, i, 0, &indirect));
		RETURN_ERROR_IF_NOT_SUCCESS(process_indirect(indirect, 0));
		RETURN_ERROR_IF_NOT_SUCCESS(IndirectObject_Release(indirect));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(File_GetDocumentCatalog(file, &catalog));
	printf("Document catalog begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &pages));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(pages, &size));

	for (i = 1; i <= size; ++i)
	{
		PageObjectHandle page = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(pages, i, &page));
		RETURN_ERROR_IF_NOT_SUCCESS(process_page(page, 1));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(pages));
	printf("Document catalog end\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_Release(xref));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	return GOTCHANG_PDF_ERROR_SUCCES;
}
