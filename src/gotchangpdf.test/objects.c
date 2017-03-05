#include "test.h"

int process_name(NameObjectHandle name, int nested) {
	BufferHandle buffer = NULL;

	print_spaces(nested);
	printf("Name object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_GetValue(name, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	printf("Name object end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

int process_lit_string(LiteralStringObjectHandle string, int nested) {
	BufferHandle buffer = NULL;

	print_spaces(nested);
	printf("Literal string begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(LiteralStringObject_GetValue(string, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	printf("Literal string end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

int process_hex_string(HexadecimalStringObjectHandle string, int nested) {
	BufferHandle buffer = NULL;

	print_spaces(nested);
	printf("Hexadecimal string begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(HexadecimalStringObject_GetValue(string, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	printf("Hexadecimal string end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

int process_dictionary(DictionaryObjectHandle dictionary, int nested) {
	boolean_type boolean = GOTCHANG_PDF_RV_FALSE;
	DictionaryObjectIteratorHandle iterator = NULL;

	print_spaces(nested);
	printf("Dictionary begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Iterator(dictionary, &iterator));
	while (GOTCHANG_PDF_ERROR_SUCCES == DictionaryObjectIterator_IsValid(iterator, dictionary, &boolean)
		&& GOTCHANG_PDF_RV_TRUE == boolean) {
		NameObjectHandle key = NULL;
		ObjectHandle value = NULL;

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
	printf("Dictionary end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_stream(StreamObjectHandle stream, int nested) {
	BufferHandle body = NULL;
	DictionaryObjectHandle dictionary = NULL;

	print_spaces(nested);
	printf("Stream object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetHeader(stream, &dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBodyRaw(stream, &body));

	RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(body, nested));

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(body));

	print_spaces(nested);
	printf("Stream object end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_array(ArrayObjectHandle arr, int nested) {
	size_type i = 0;
	size_type size = 0;

	print_spaces(nested);
	printf("Array begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Size(arr, &size));

	print_spaces(nested + 1);
	unsigned long long converted_size = size;
	printf("Size: %llu\n", converted_size);

	for (i = 0; i < size; ++i) {
		ObjectHandle item = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_At(arr, i, &item));
		RETURN_ERROR_IF_NOT_SUCCESS(process_object(item, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(item));
	}

	print_spaces(nested);
	printf("Array end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_integer(IntegerObjectHandle integer, int nested) {
	bigint_type value = 0;

	print_spaces(nested);
	printf("Integer object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_GetIntegerValue(integer, &value));
	print_spaces(nested + 1);

	long long converted = value;
	printf("Value: %lld\n", converted);

	print_spaces(nested);
	printf("Integer object end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_boolean(BooleanObjectHandle obj, int nested) {
	boolean_type value = GOTCHANG_PDF_RV_FALSE;

	print_spaces(nested);
	printf("Boolean object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(BooleanObject_GetValue(obj, &value));

	print_spaces(nested + 1);
	if (GOTCHANG_PDF_RV_TRUE == value)
		printf("Value: true\n");
	else
		printf("Value: false\n");

	print_spaces(nested);
	printf("Boolean object end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_reference(IndirectObjectReferenceHandle reference, int nested) {
	ObjectType type;
	biguint_type obj_num = 0;
	ushort_type gen_num = 0;
	ObjectHandle child = NULL;
	string_type type_name = NULL;

	print_spaces(nested);
	printf("Indirect reference begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(IndirectReference_GetReferencedObjectNumber(reference, &obj_num));
	RETURN_ERROR_IF_NOT_SUCCESS(IndirectReference_GetReferencedGenerationNumber(reference, &gen_num));
	RETURN_ERROR_IF_NOT_SUCCESS(IndirectReference_GetReferencedObject(reference, &child));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(child, &type));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_TypeName(type, &type_name));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(child));

	print_spaces(nested + 1);
	unsigned long long obj_num_converted = obj_num;
	printf("Object Number: %llu\n", obj_num_converted);

	print_spaces(nested + 1);
	printf("Generation Number: %d\n", gen_num);

	print_spaces(nested + 1);
	printf("Type: %s\n", type_name);

	print_spaces(nested);
	printf("Indirect reference end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_real(RealObjectHandle obj, int nested) {
	real_type value = 0;

	print_spaces(nested);
	printf("Real object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(RealObject_GetValue(obj, &value));
	print_spaces(nested + 1);
	printf("Value: %g\n", value);

	print_spaces(nested);
	printf("Real object end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

error_type process_null(NullObjectHandle obj, int nested) {
	print_spaces(nested);
	printf("Null object begin\n");

	print_spaces(nested + 1);
	printf("Value: null\n");

	print_spaces(nested);
	printf("Null object end\n");

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

int process_string(StringObjectHandle obj, int nested) {
	StringType type;
	LiteralStringObjectHandle literal_str = NULL;
	HexadecimalStringObjectHandle hexadecimal_str = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Type(obj, &type));

	switch (type) {
		case StringType_Literal:
			RETURN_ERROR_IF_NOT_SUCCESS(StringObject_ToLiteral(obj, &literal_str));
			RETURN_ERROR_IF_NOT_SUCCESS(process_lit_string(literal_str, nested));
			break;
		case StringType_Hexadecimal:
			RETURN_ERROR_IF_NOT_SUCCESS(StringObject_ToHexadecimal(obj, &hexadecimal_str));
			RETURN_ERROR_IF_NOT_SUCCESS(process_hex_string(hexadecimal_str, nested));
			break;
		default:
			print_spaces(nested);
			printf("Unknown string type\n");
			return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}

int process_object(ObjectHandle obj, int nested) {
	ObjectType type;
	RealObjectHandle real = NULL;
	BooleanObjectHandle boolean = NULL;
	NullObjectHandle null_object = NULL;
	IndirectObjectReferenceHandle indirect_reference = NULL;
	ArrayObjectHandle arr = NULL;
	IntegerObjectHandle integer = NULL;
	StreamObjectHandle stream = NULL;
	NameObjectHandle name = NULL;
	ObjectHandle child = NULL;
	DictionaryObjectHandle dictionary = NULL;
	StringObjectHandle string = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetType(obj, &type));

	switch (type) {
		case ObjectType_Array:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToArray(obj, &arr));
			RETURN_ERROR_IF_NOT_SUCCESS(process_array(arr, nested));
			break;
		case ObjectType_Boolean:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToBoolean(obj, &boolean));
			RETURN_ERROR_IF_NOT_SUCCESS(process_boolean(boolean, nested));
			break;
		case ObjectType_Dictionary:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToDictionary(obj, &dictionary));
			RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
			break;
		case ObjectType_Integer:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToInteger(obj, &integer));
			RETURN_ERROR_IF_NOT_SUCCESS(process_integer(integer, nested));
			break;
		case ObjectType_Name:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToName(obj, &name));
			RETURN_ERROR_IF_NOT_SUCCESS(process_name(name, nested));
			break;
		case ObjectType_Null:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToNull(obj, &null_object));
			RETURN_ERROR_IF_NOT_SUCCESS(process_null(null_object, nested));
			break;
		case ObjectType_Real:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToReal(obj, &real));
			RETURN_ERROR_IF_NOT_SUCCESS(process_real(real, nested));
			break;
		case ObjectType_Stream:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToStream(obj, &stream));
			RETURN_ERROR_IF_NOT_SUCCESS(process_stream(stream, nested));
			break;
		case ObjectType_String:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToString(obj, &string));
			RETURN_ERROR_IF_NOT_SUCCESS(process_string(string, nested));
			break;
		case ObjectType_IndirectReference:
			RETURN_ERROR_IF_NOT_SUCCESS(Object_ToIndirectReference(obj, &indirect_reference));
			RETURN_ERROR_IF_NOT_SUCCESS(process_reference(indirect_reference, nested));
			break;
		default:
			print_spaces(nested);
			printf("Unknown object type\n");
			return GOTCHANG_PDF_TEST_ERROR_FAILURE;
	}

	return GOTCHANG_PDF_TEST_ERROR_SUCCESS;
}
