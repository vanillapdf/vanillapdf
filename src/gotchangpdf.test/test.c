#include "gotchangpdf/gotchangpdf.h"

#include <stdio.h>
#include <assert.h>

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
	int i, size;
	long long val;
	const char *name;
	ObjectHandle child;
	ArrayObjectHandle arr;
	IntegerObjectHandle integer;
	DictionaryObjectHandle dict;
	DictionaryObjectIteratorHandle iterator;

	enum ObjectType type = Object_Type(obj);

	switch (type)
	{
	case Unknown:
		print_spaces(nested);
		printf("UNRECOGNIZED OBJECT TYPE!\n");

		assert(0);

		break;
	case ArrayObject:
		arr = (ArrayObjectHandle)obj;
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
	case DictionaryObject:
		dict = (DictionaryObjectHandle)obj;
		print_spaces(nested);
		printf("Dictionary begin\n");
		iterator = DictionaryObject_Iterator(dict);
		DictionaryObjectIterator_Next(iterator);
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
	case IntegerObject:
		print_spaces(nested);
		printf("Integer object begin\n");

		integer = (IntegerObjectHandle)obj;
		val = IntegerObject_Value(integer);
		print_spaces(nested + 1);
		printf("Value: %lld\n", val);

		print_spaces(nested);
		printf("Integer object end\n");
		break;
	case NameObject:
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
	case NullObject:
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
	case RealObject:
		print_spaces(nested);
		printf("Real object begin\n");

		print_spaces(nested);
		printf("Real object end\n");
		break;
	case StreamObject:
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
	case IndirectObjectReference:
		print_spaces(nested);
		printf("Indirect reference begin\n");

		process(IndirectReference_GetReferencedObject((IndirectObjectReferenceHandle)obj), nested + 1);

		print_spaces(nested);
		printf("Indirect reference end\n");
		break;
	case IndirectObject:
		print_spaces(nested);
		printf("Indirect object begin\n");

		print_spaces(nested + 1);
		printf("Offset: %d\n\n", IndirectObject_GetOffset((IndirectObjectHandle)obj));

		child = IndirectObject_GetObject((IndirectObjectHandle)obj);
		process(child, nested + 1);
		Object_Release(child);

		print_spaces(nested);
		printf("Indirect object end\n");
		break;
	default:
		break;
	}
}

int main()
{
	int i, result, size;
	FileHandle file;
	XrefHandle xref;
	CatalogHandle catalog;
	PageTreeHandle pages;
	ArrayObjectHandle page_kids;
	int page_count;
	const char str[] = "test\\manual-memorias.pdf";
	//const char str[] = "test\\pdfSample.pdf";

	file = File_Create(str);
	result = File_Initialize(file);
	xref = File_Xref(file);

	size = Xref_Size(xref);

	for (i = 1; i < size; ++i)
	{
		IndirectObjectHandle indirect = File_GetIndirectObject(file, i, 0);
		process((ObjectHandle)indirect, 0);
		IndirectObject_Release(indirect);
	}

	catalog = File_GetDocumentCatalog(file);
	printf("Document catalog begin\n");

	pages = Catalog_GetPages(catalog);
	size = PageTree_GetPageCount(pages);

	for (i = 0; i < size; ++i)
	{
		PageObjectHandle page = PageTree_GetPage(pages, i);
		process_page(page, 1);
		PageObject_Release(page);
	}

	//PageTree_Release(pages);

	printf("Document catalog end\n");

	Xref_Release(xref);
	File_Release(file);

	getchar();
}
