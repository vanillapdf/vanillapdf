#include "test.h"

error_type process_name(NameObjectHandle* name, int nested) {
	BufferHandle* buffer = NULL;

	print_spaces(nested);
	print_text("Name object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(NameObject_GetValue(name, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	print_text("Name object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_lit_string(LiteralStringObjectHandle* string, int nested) {
	BufferHandle* buffer = NULL;

	print_spaces(nested);
	print_text("Literal string begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(LiteralStringObject_GetValue(string, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	print_text("Literal string end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_hex_string(HexadecimalStringObjectHandle* string, int nested) {
	BufferHandle* buffer = NULL;

	print_spaces(nested);
	print_text("Hexadecimal string begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(HexadecimalStringObject_GetValue(string, &buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(buffer, nested + 1));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer));

	print_spaces(nested);
	print_text("Hexadecimal string end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_dictionary(DictionaryObjectHandle* dictionary, int nested) {
	boolean_type boolean = VANILLAPDF_RV_FALSE;
	DictionaryObjectIteratorHandle* iterator = NULL;

	print_spaces(nested);
	print_text("Dictionary begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_GetIterator(dictionary, &iterator));
	while (VANILLAPDF_ERROR_SUCCESS == DictionaryObjectIterator_IsValid(iterator, &boolean)
		&& VANILLAPDF_RV_TRUE == boolean) {
		NameObjectHandle* key = NULL;
		ObjectHandle* value = NULL;

		print_spaces(nested);
		print_text("Pair:\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_GetKey(iterator, &key));
		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_GetValue(iterator, &value));

		RETURN_ERROR_IF_NOT_SUCCESS(process_name(key, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(process_object(value, nested + 1));

		RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(key));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(value));

		print_spaces(nested);
		print_text("EndPair\n");

		RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_Next(iterator));
	}

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObjectIterator_Release(iterator));

	print_spaces(nested);
	print_text("Dictionary end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_stream(StreamObjectHandle* stream, int nested) {
	BufferHandle* body = NULL;
	DictionaryObjectHandle* dictionary = NULL;

	print_spaces(nested);
	print_text("Stream object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetHeader(stream, &dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBodyRaw(stream, &body));

	RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
	RETURN_ERROR_IF_NOT_SUCCESS(process_buffer(body, nested));

	RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(body));

	print_spaces(nested);
	print_text("Stream object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_array(ArrayObjectHandle* arr, int nested) {
	size_type i = 0;
	size_type size = 0;

	print_spaces(nested);
	print_text("Array begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_GetSize(arr, &size));

	print_spaces(nested + 1);
	unsigned long long converted_size = size;
	print_text("Size: %llu\n", converted_size);

	for (i = 0; i < size; ++i) {
		ObjectHandle* item = NULL;
		RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_GetValue(arr, i, &item));
		RETURN_ERROR_IF_NOT_SUCCESS(process_object(item, nested + 1));
		RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(item));
	}

	print_spaces(nested);
	print_text("Array end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_integer(IntegerObjectHandle* integer, int nested) {
	bigint_type value = 0;

	print_spaces(nested);
	print_text("Integer object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_GetIntegerValue(integer, &value));
	print_spaces(nested + 1);

	long long converted = value;
	print_text("Value: %lld\n", converted);

	print_spaces(nested);
	print_text("Integer object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_boolean(BooleanObjectHandle* obj, int nested) {
	boolean_type value = VANILLAPDF_RV_FALSE;

	print_spaces(nested);
	print_text("Boolean object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(BooleanObject_GetValue(obj, &value));

	print_spaces(nested + 1);
	if (VANILLAPDF_RV_TRUE == value) {
		print_text("Value: true\n");
	}
	else {
		print_text("Value: false\n");
	}

	print_spaces(nested);
	print_text("Boolean object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_reference(IndirectReferenceObjectHandle* reference, int nested) {
	ObjectType type;
	biguint_type obj_num = 0;
	ushort_type gen_num = 0;
	ObjectHandle* child = NULL;
	string_type type_name = NULL;

	print_spaces(nested);
	print_text("Indirect reference begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(IndirectReferenceObject_GetReferencedObjectNumber(reference, &obj_num));
	RETURN_ERROR_IF_NOT_SUCCESS(IndirectReferenceObject_GetReferencedGenerationNumber(reference, &gen_num));
	RETURN_ERROR_IF_NOT_SUCCESS(IndirectReferenceObject_GetReferencedObject(reference, &child));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(child, &type));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_TypeName(type, &type_name));
	RETURN_ERROR_IF_NOT_SUCCESS(Object_Release(child));

	print_spaces(nested + 1);
	unsigned long long obj_num_converted = obj_num;
	print_text("Object Number: %llu\n", obj_num_converted);

	print_spaces(nested + 1);
	print_text("Generation Number: %d\n", gen_num);

	print_spaces(nested + 1);
	print_text("Type: %s\n", type_name);

	print_spaces(nested);
	print_text("Indirect reference end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_real(RealObjectHandle* obj, int nested) {
	real_type value = 0;

	print_spaces(nested);
	print_text("Real object begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(RealObject_GetValue(obj, &value));
	print_spaces(nested + 1);
	print_text("Value: %g\n", value);

	print_spaces(nested);
	print_text("Real object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_null(NullObjectHandle* obj, int nested) {

	// Null object does not have any properties
	UNUSED(obj);

	print_spaces(nested);
	print_text("Null object begin\n");

	print_spaces(nested + 1);
	print_text("Value: null\n");

	print_spaces(nested);
	print_text("Null object end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_string(StringObjectHandle* obj, int nested) {
	StringType type;
	LiteralStringObjectHandle* literal_str = NULL;
	HexadecimalStringObjectHandle* hexadecimal_str = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(StringObject_GetType(obj, &type));

	switch (type) {
		case StringType_Literal:
			RETURN_ERROR_IF_NOT_SUCCESS(LiteralStringObject_FromStringObject(obj, &literal_str));
			RETURN_ERROR_IF_NOT_SUCCESS(process_lit_string(literal_str, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(LiteralStringObject_Release(literal_str));
			break;
		case StringType_Hexadecimal:
			RETURN_ERROR_IF_NOT_SUCCESS(HexadecimalStringObject_FromStringObject(obj, &hexadecimal_str));
			RETURN_ERROR_IF_NOT_SUCCESS(process_hex_string(hexadecimal_str, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(HexadecimalStringObject_Release(hexadecimal_str));
			break;
		default:
			print_spaces(nested);
			print_text("Unknown string type\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

error_type process_object(ObjectHandle* obj, int nested) {
	ObjectType type;
	RealObjectHandle* real = NULL;
	BooleanObjectHandle* boolean = NULL;
	NullObjectHandle* null_object = NULL;
	IndirectReferenceObjectHandle* indirect_reference = NULL;
	ArrayObjectHandle* arr = NULL;
	IntegerObjectHandle* integer = NULL;
	StreamObjectHandle* stream = NULL;
	NameObjectHandle* name = NULL;
	DictionaryObjectHandle* dictionary = NULL;
	StringObjectHandle* string = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(obj, &type));

	switch (type) {
		case ObjectType_Array:
			RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_FromObject(obj, &arr));
			RETURN_ERROR_IF_NOT_SUCCESS(process_array(arr, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(ArrayObject_Release(arr));
			break;
		case ObjectType_Boolean:
			RETURN_ERROR_IF_NOT_SUCCESS(BooleanObject_FromObject(obj, &boolean));
			RETURN_ERROR_IF_NOT_SUCCESS(process_boolean(boolean, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(BooleanObject_Release(boolean));
			break;
		case ObjectType_Dictionary:
			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_FromObject(obj, &dictionary));
			RETURN_ERROR_IF_NOT_SUCCESS(process_dictionary(dictionary, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Release(dictionary));
			break;
		case ObjectType_Integer:
			RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_FromObject(obj, &integer));
			RETURN_ERROR_IF_NOT_SUCCESS(process_integer(integer, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(IntegerObject_Release(integer));
			break;
		case ObjectType_Name:
			RETURN_ERROR_IF_NOT_SUCCESS(NameObject_FromObject(obj, &name));
			RETURN_ERROR_IF_NOT_SUCCESS(process_name(name, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Release(name));
			break;
		case ObjectType_Null:
			RETURN_ERROR_IF_NOT_SUCCESS(NullObject_FromObject(obj, &null_object));
			RETURN_ERROR_IF_NOT_SUCCESS(process_null(null_object, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(NullObject_Release(null_object));
			break;
		case ObjectType_Real:
			RETURN_ERROR_IF_NOT_SUCCESS(RealObject_FromObject(obj, &real));
			RETURN_ERROR_IF_NOT_SUCCESS(process_real(real, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(RealObject_Release(real));
			break;
		case ObjectType_Stream:
			RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_FromObject(obj, &stream));
			RETURN_ERROR_IF_NOT_SUCCESS(process_stream(stream, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_Release(stream));
			break;
		case ObjectType_String:
			RETURN_ERROR_IF_NOT_SUCCESS(StringObject_FromObject(obj, &string));
			RETURN_ERROR_IF_NOT_SUCCESS(process_string(string, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(StringObject_Release(string));
			break;
		case ObjectType_IndirectReference:
			RETURN_ERROR_IF_NOT_SUCCESS(IndirectReferenceObject_FromObject(obj, &indirect_reference));
			RETURN_ERROR_IF_NOT_SUCCESS(process_reference(indirect_reference, nested));
			RETURN_ERROR_IF_NOT_SUCCESS(IndirectReferenceObject_Release(indirect_reference));
			break;
		default:
			print_spaces(nested);
			print_text("Unknown object type\n");
			return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
