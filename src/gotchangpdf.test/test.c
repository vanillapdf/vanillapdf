#include "gotchangpdf/gotchangpdf.h"

#include <stdio.h>
#include <assert.h>

#define RETURN_ERROR_IF_NOT_SUCCESS(var) do { int __result__ = (var);  if (GOTCHANG_PDF_ERROR_SUCCES != __result__) return __result__; } while(0)

void process(ObjectHandle obj, int nested);

void print_spaces(int nested)
{
	int i;
	for (i = 0; i < nested; ++i)
		printf("  ");
}

void process_page(PageObjectHandle obj, int nested)
{
	print_spaces(nested);
	printf("Page begin\n");

	print_spaces(nested);
	printf("Page end\n");
}

void process(ObjectHandle obj, int nested)
{
	int i, size, val;
	ObjectHandle child;
	ArrayHandle arr;
	IntegerHandle integer;
	DictionaryHandle dict;
	DictionaryIteratorHandle iterator;

	enum ObjectType type = Object_Type(obj);

	if (nested > 2)
		return;

	switch (type)
	{
	case Unknown:
		print_spaces(nested);
		printf("UNRECOGNIZED OBJECT TYPE!\n");

		assert(0);

		break;
	case Array:
		arr = (ArrayHandle)obj;
		size = ArrayObject_Size(arr);
		print_spaces(nested);
		printf("Array begin\n");

		print_spaces(nested + 1);
		printf("Size: %d\n\n", size);
		for (i = 0; i < size; ++i)
		{
			child = ArrayObject_At(arr, i);
			process(child, nested + 1);
			Object_Release(child);
		}

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
		dict = (DictionaryHandle)obj;
		print_spaces(nested);
		printf("Dictionary begin\n");
		iterator = DictionaryObject_Iterator(dict);
		while (GOTCHANG_PDF_RV_TRUE == DictionaryObjectIterator_IsValid(iterator, dict))
		{
			print_spaces(nested);
			printf("Pair:\n");

			process((ObjectHandle)DictionaryObjectIterator_GetKey(iterator), nested + 1);
			process((ObjectHandle)DictionaryObjectIterator_GetValue(iterator), nested + 1);

			print_spaces(nested);
			printf("EndPair\n");

			DictionaryObjectIterator_Next(iterator);
		}

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

		integer = (IntegerHandle)obj;
		val = IntegerObject_Value(integer);
		print_spaces(nested + 1);
		printf("Value: %d\n", val);

		print_spaces(nested);
		printf("Integer object end\n");
		break;
	case Name:
		print_spaces(nested);
		printf("Name object begin\n");

		print_spaces(nested);
		printf("Name object end\n");
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

		process((ObjectHandle)IndirectReference_GetReferencedObject((IndirectReferenceHandle)obj), nested + 1);

		print_spaces(nested);
		printf("Indirect reference end\n");
		break;
	case Indirect:
		print_spaces(nested);
		printf("Indirect object begin\n");

		print_spaces(nested + 1);
		printf("Offset: %d\n\n", IndirectObject_GetOffset((IndirectHandle)obj));

		child = IndirectObject_GetObject((IndirectHandle)obj);
		process(child, nested + 1);
		Object_Release(child);

		print_spaces(nested);
		printf("Indirect object end\n");
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	int i, size;
	FileHandle file = NULL;
	XrefHandle xref = NULL;
	//CatalogHandle catalog;
	//PageTreeHandle pages;

	if (argc != 2)
		return 1;

	RETURN_ERROR_IF_NOT_SUCCESS(File_Create(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Xref(file, &xref));
	size = Xref_Size(xref);

	for (i = 1; i < size; ++i)
	{
		IndirectHandle indirect = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(File_GetIndirectObject(file, i, 0, &indirect));

		process((ObjectHandle)indirect, 0);
		IndirectObject_Release(indirect);
	}
	/*
	catalog = File_GetDocumentCatalog(file);
	printf("Document catalog begin\n");

	pages = Catalog_GetPages(catalog);
	size = PageTree_GetPageCount(pages);

	for (i = 0; i < size; ++i)
	{
		PageObjectHandle page = PageTree_GetPage(pages, i + 1);
		process_page(page, 1);
		PageObject_Release(page);
	}

	PageTree_Release(pages);
	*/
	printf("Document catalog end\n");

	Xref_Release(xref);
	File_Release(file);

	return 0;
}
