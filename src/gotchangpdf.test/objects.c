#include "test.h"

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
		RETURN_ERROR_IF_NOT_SUCCESS(process_object(value, nested + 1));

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

int process_object(ObjectHandle obj, int nested)
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
	case ObjectType_Array:
		print_spaces(nested);
		printf("Array begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToArray(obj, &arr));
		RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Size(arr, &size));

		print_spaces(nested + 1);
		printf("Size: %d\n", size);
		for (i = 0; i < size; ++i)
		{
			RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_At(arr, i, &child));
			RETURN_ERROR_IF_NOT_SUCCESS(process_object(child, nested + 1));
			RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(child));
		}

		print_spaces(nested);
		printf("Array end\n");

		break;
	case ObjectType_Boolean:
		print_spaces(nested);
		printf("Boolean object begin\n");

		print_spaces(nested);
		printf("Boolean object end\n");
		break;
	case ObjectType_Dictionary:
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToDictionary(obj, &dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
		break;
	case ObjectType_Function:
		print_spaces(nested);
		printf("Function object begin\n");

		print_spaces(nested);
		printf("Function object end\n");
		break;
	case ObjectType_Integer:
		print_spaces(nested);
		printf("Integer object begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToInteger(obj, &integer));
		RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Value(integer, &val));
		print_spaces(nested + 1);
		printf("Value: %d\n", val);

		print_spaces(nested);
		printf("Integer object end\n");
		break;
	case ObjectType_Name:
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToName(obj, &name));
		RETURN_ERROR_IF_NOT_SUCCESS(process_name(name, nested));
		break;
	case ObjectType_Null:
		print_spaces(nested);
		printf("Null object begin\n");

		printf("Value: null\n");

		print_spaces(nested);
		printf("Null object end\n");
		break;
	case ObjectType_Real:
		print_spaces(nested);
		printf("Real object begin\n");

		print_spaces(nested);
		printf("Real object end\n");
		break;
	case ObjectType_Stream:
		print_spaces(nested);
		printf("Stream object begin\n");

		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToStream(obj, &stream));
		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_Header(stream, &dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_BodyDecoded(stream, &body_decoded));

		RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
		RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(body_decoded, nested));

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));
		RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(body_decoded));

		print_spaces(nested);
		printf("Stream object end\n");
		break;
	case ObjectType_HexadecimalString:
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToHexadecimalString(obj, &hex_string));
		RETURN_ERROR_IF_NOT_SUCCESS(process_hex_string(hex_string, nested));
		break;
	case ObjectType_LiteralString:
		RETURN_ERROR_IF_NOT_SUCCESS(Object_ToLiteralString(obj, &literal_string));
		RETURN_ERROR_IF_NOT_SUCCESS(process_lit_string(literal_string, nested));
		break;
	case ObjectType_IndirectReference:
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
		print_spaces(nested);
		printf("Unknown object type\n");
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}