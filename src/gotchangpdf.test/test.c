#include "gotchangpdf/gotchangpdf.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

#define RETURN_ERROR_IF_NOT_SUCCESS(var) do { int __result__ = (var);  if (GOTCHANG_PDF_ERROR_SUCCES != __result__) return __result__; } while(0)

int process(ObjectHandle obj, int nested);

void print_spaces(int nested)
{
	int i;
	for (i = 0; i < nested; ++i)
		printf("  ");
}

int process_contents(ContentsHandle obj, int nested)
{
	print_spaces(nested);
	printf("Contents begin\n");

	print_spaces(nested);
	printf("Contents end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_page(PageObjectHandle obj, int nested)
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

int process_buffer(BufferHandle buffer, int nested)
{
	string_type data;
	char* local_string;
	size_type size;
	size_t size_converted;
	size_t print_size;

	print_spaces(nested);
	printf("Buffer begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetSize(buffer, &size));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(buffer, &data));

	if (size >= SIZE_MAX)
		return GOTCHANG_PDF_ERROR_GENERAL;

	size_converted = (size_t)size;
	print_size = size_converted > 20 ? 10 : size_converted;

	local_string = calloc(sizeof(char), print_size + 1);
	if (NULL == local_string)
		return GOTCHANG_PDF_ERROR_GENERAL;

	memcpy(local_string, data, print_size);

	print_spaces(nested + 1);
	printf("Size: %d\n", size_converted);
	print_spaces(nested + 1);
	printf("Data: %s\n", local_string);

	free(local_string);

	print_spaces(nested);
	printf("Buffer end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_name(NameHandle name, int nested)
{
	BufferHandle buffer;

	print_spaces(nested);
	printf("Name object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Value(name, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	printf("Name object end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_lit_string(LiteralStringHandle string, int nested)
{
	BufferHandle buffer;

	print_spaces(nested);
	printf("Literal string begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(LiteralStringObject_Value(string, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	printf("Literal string end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_hex_string(HexadecimalStringHandle string, int nested)
{
	BufferHandle buffer;

	print_spaces(nested);
	printf("Hexadecimal string begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(HexadecimalStringObject_Value(string, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	printf("Hexadecimal string end\n");

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_dictionary(DictionaryHandle dictionary, int nested)
{
	int boolean;
	DictionaryIteratorHandle iterator;
	NameHandle key;
	ObjectHandle value;

	print_spaces(nested);
	printf("Dictionary begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Iterator(dictionary, &iterator));
	while (GOTCHANG_PDF_ERROR_SUCCES == DictionaryObjectIterator_IsValid(iterator, dictionary, &boolean)
		&& GOTCHANG_PDF_RV_TRUE == boolean)
	{
		print_spaces(nested);
		printf("Pair:\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_GetKey(iterator, &key));
		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_GetValue(iterator, &value));

		RETURN_ERROR_IF_NOT_SUCCESS(process_name(key, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(process(value, nested + 1));

		RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(key));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(value));

		print_spaces(nested);
		printf("EndPair\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_Next(iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_Release(iterator));

	print_spaces(nested);
	printf("Dictionary End\n");
	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process(ObjectHandle obj, int nested)
{
	int i, size, val, obj_num, gen_num;
	BufferHandle body_decoded;
	IndirectReferenceHandle indirect_reference;
	ArrayHandle arr;
	IntegerHandle integer;
	StreamHandle stream;
	NameHandle name;
	ObjectHandle child;
	DictionaryHandle dictionary;
	LiteralStringHandle literal_string;
	HexadecimalStringHandle hex_string;
	enum ObjectType type;
	string_type type_name;

	RETURN_ERROR_IF_NOT_SUCCESS(Object_Type(obj, &type));

	if (nested > 2)
		return GOTCHANG_PDF_ERROR_SUCCES;

	switch (type)
	{
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
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToDictionary(obj, &dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
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
	case Null:
		print_spaces(nested);
		printf("Null object begin\n");

		printf("Value: null\n");

		print_spaces(nested);
		printf("Null object end\n");
		break;
	case Real:
		print_spaces(nested);
		printf("Real object begin\n");

		print_spaces(nested);
		printf("Real object end\n");
		break;
	case Stream:
		print_spaces(nested);
		printf("Stream object begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToStream(obj, &stream));
		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_Header(stream, &dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_BodyDecoded(stream, &body_decoded));

		RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
		RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(body_decoded, nested));

		print_spaces(nested);
		printf("Stream object end\n");
		break;
	case HexadecimalString:
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToHexadecimalString(obj, &hex_string));
		RETURN_ERROR_IF_NOT_SUCCESS(process_hex_string(hex_string, nested));
		break;
	case LiteralString:
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToLiteralString(obj, &literal_string));
		RETURN_ERROR_IF_NOT_SUCCESS(process_lit_string(literal_string, nested));
		break;
	case IndirectReference:
		print_spaces(nested);
		printf("Indirect reference begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToIndirectReference(obj, &indirect_reference));

		RETURN_ERROR_IF_NOT_SUCCESS(IndirectReference_GetReferencedObjectNumber(indirect_reference, &obj_num));
		RETURN_ERROR_IF_NOT_SUCCESS(IndirectReference_GetReferencedGenerationNumber(indirect_reference, &gen_num));
		RETURN_ERROR_IF_NOT_SUCCESS(IndirectReference_GetReferencedObject(indirect_reference, &child));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Type(child, &type));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_TypeName(type, &type_name));

		print_spaces(nested + 1);
		printf("Object Number: %d\n", obj_num);

		print_spaces(nested + 1);
		printf("Generation Number: %d\n", gen_num);

		print_spaces(nested + 1);
		printf("Type: %s\n", type_name);

		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(child));

		print_spaces(nested);
		printf("Indirect reference end\n");
		break;
	default:
		printf("Unknown object type\n");
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int process_xref(XrefHandle xref)
{
	int i, size;

	RETURN_ERROR_IF_NOT_SUCCESS(Xref_Size(xref, &size));

	for (i = 0; i < size; ++i) {
		int j, subsection_size;
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
					RETURN_ERROR_IF_NOT_SUCCESS(process(obj, 0));
					RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(obj));
					break;
				case XrefEntryCompressed:
					RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_ToCompressedEntry(entry, &compressed_entry));
					RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_Reference(compressed_entry, &obj));
					RETURN_ERROR_IF_NOT_SUCCESS(process(obj, 0));
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
		}
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

int main(int argc, char *argv[])
{
	int i, valid, size;
	FileHandle file = NULL;
	XrefChainHandle chain = NULL;
	XrefChainIteratorHandle chain_iterator = NULL;
	CatalogHandle catalog = NULL;
	PageTreeHandle pages = NULL;

	if (argc != 2)
		return GOTCHANG_PDF_ERROR_GENERAL;

	RETURN_ERROR_IF_NOT_SUCCESS(File_Open(argv[1], &file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));
	RETURN_ERROR_IF_NOT_SUCCESS(File_XrefChain(file, &chain));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Iterator(chain, &chain_iterator));

	while (GOTCHANG_PDF_ERROR_SUCCES == XrefChainIterator_IsValid(chain_iterator, chain, &valid)
		&& GOTCHANG_PDF_RV_TRUE == valid) {
		XrefHandle xref = NULL;

		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_GetValue(chain_iterator, &xref));

		RETURN_ERROR_IF_NOT_SUCCESS(process_xref(xref));

		RETURN_ERROR_IF_NOT_SUCCESS(Xref_Release(xref));
		RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Next(chain_iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(File_GetDocumentCatalog(file, &catalog));
	printf("Document catalog begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, &pages));
	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(pages, &size));

	for (i = 1; i <= size; ++i)
	{
		PageObjectHandle page = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(pages, i, &page));
		RETURN_ERROR_IF_NOT_SUCCESS(process_page(page, 0));
		RETURN_ERROR_IF_NOT_SUCCESS(PageObject_Release(page));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(PageTree_Release(pages));
	printf("Document catalog end\n");

	
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Release(chain_iterator));
	RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_Release(chain));
	RETURN_ERROR_IF_NOT_SUCCESS(File_Release(file));

	return GOTCHANG_PDF_ERROR_SUCCES;
}
