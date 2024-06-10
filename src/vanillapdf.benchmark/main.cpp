#include "benchmark.h"

template <class ...Args>
static void BM_RealObjectToPdf(benchmark::State& state, Args&&... args) {
	auto args_tuple = std::make_tuple(std::move(args)...);

	for (auto _ : state) {
		RealObjectHandle* real_object = nullptr;
		ObjectHandle* base_object = nullptr;
		BufferHandle* object_pdf_buffer = nullptr;

		// Create data from defined value
		RealObject_CreateFromData(std::get<0>(args_tuple), std::get<1>(args_tuple), &real_object);

		// Convert to base object
		RealObject_ToObject(real_object, &base_object);

		// Get PDF representation of RealObject value
		Object_ToPdf(base_object, &object_pdf_buffer);

		// Cleanup
		Buffer_Release(object_pdf_buffer);
		Object_Release(base_object);
		RealObject_Release(real_object);
	}
}

BENCHMARK_CAPTURE(BM_RealObjectToPdf, no_precision, 752.43f, 0);
BENCHMARK_CAPTURE(BM_RealObjectToPdf, int_precision, 752.43f, 2);

template <class ...Args>
static void BM_HexadecimalStringObjectToPdf(benchmark::State& state, Args&&... args) {
	auto args_tuple = std::make_tuple(std::move(args)...);

	for (auto _ : state) {
		HexadecimalStringObjectHandle* hex_string_object = nullptr;
		StringObjectHandle* string_object = nullptr;
		ObjectHandle* base_object = nullptr;
		BufferHandle* object_pdf_buffer = nullptr;

		std::string encoded_hex_string = std::get<0>(args_tuple);

		// Create data from defined value
		HexadecimalStringObject_CreateFromEncodedString(encoded_hex_string.c_str(), &hex_string_object);

		// Get base object as it is the only one having ToPdf exposed
		HexadecimalStringObject_ToStringObject(hex_string_object, &string_object);
		StringObject_ToObject(string_object, &base_object);

		// Get PDF representation of HexadecimalStringObject value
		Object_ToPdf(base_object, &object_pdf_buffer);

		// Cleanup
		Buffer_Release(object_pdf_buffer);
		Object_Release(base_object);
		StringObject_Release(string_object);
		HexadecimalStringObject_Release(hex_string_object);
	}
}

BENCHMARK_CAPTURE(BM_HexadecimalStringObjectToPdf, string_empty, "");
BENCHMARK_CAPTURE(BM_HexadecimalStringObjectToPdf, string_unpaired, "F");
BENCHMARK_CAPTURE(BM_HexadecimalStringObjectToPdf, string_values, "000A0FF0FF");

template <class ...Args>
static void BM_LiteralStringObjectToPdf(benchmark::State& state, Args&&... args) {
	auto args_tuple = std::make_tuple(std::move(args)...);

	for (auto _ : state) {
		LiteralStringObjectHandle* literal_string_object = nullptr;
		StringObjectHandle* string_object = nullptr;
		ObjectHandle* base_object = nullptr;
		BufferHandle* object_pdf_buffer = nullptr;

		std::string encoded_literal_string = std::get<0>(args_tuple);

		// Create data from defined value
		LiteralStringObject_CreateFromEncodedString(encoded_literal_string.c_str(), &literal_string_object);

		// Get base object as it is the only one having ToPdf exposed
		LiteralStringObject_ToStringObject(literal_string_object, &string_object);
		StringObject_ToObject(string_object, &base_object);

		// Get PDF representation of LiteralStringObject value
		Object_ToPdf(base_object, &object_pdf_buffer);

		// Cleanup
		Buffer_Release(object_pdf_buffer);
		Object_Release(base_object);
		StringObject_Release(string_object);
		LiteralStringObject_Release(literal_string_object);
	}
}

BENCHMARK_CAPTURE(BM_LiteralStringObjectToPdf, string_empty, "");
BENCHMARK_CAPTURE(BM_LiteralStringObjectToPdf, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_LiteralStringObjectToPdf, string_octal, "\001\002\003\004\252\253\254\255");

template <class ...Args>
static void BM_NameObjectToPdf(benchmark::State& state, Args&&... args) {
	auto args_tuple = std::make_tuple(std::move(args)...);

	for (auto _ : state) {
		NameObjectHandle* name_object = nullptr;
		ObjectHandle* base_object = nullptr;
		BufferHandle* object_pdf_buffer = nullptr;

		std::string encoded_name = std::get<0>(args_tuple);

		// Create data from defined value
		NameObject_CreateFromEncodedString(encoded_name.c_str(), &name_object);

		// Get base object as it is the only one having ToPdf exposed
		NameObject_ToObject(name_object, &base_object);

		// Get PDF representation of LiteralStringObject value
		Object_ToPdf(base_object, &object_pdf_buffer);

		// Cleanup
		Buffer_Release(object_pdf_buffer);
		Object_Release(base_object);
		NameObject_Release(name_object);
	}
}

BENCHMARK_CAPTURE(BM_NameObjectToPdf, string_empty, "");
BENCHMARK_CAPTURE(BM_NameObjectToPdf, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_NameObjectToPdf, string_hexadecimal, "#01#02#03#FA#FB#FC#FD#FE#FF");

static const char SAMPLE_CONTENT_STREAM[] = R"(q
0.000008871 0 595.32 841.92 re
W* n
BT
/F1 9 Tf
1 0 0 1 362.83 799.8 Tm
0 g
0 G
[( )] TJ
ET
Q
q
0.000008871 0 595.32 841.92 re
W* n
BT
/F1 9 Tf
1 0 0 1 458.35 799.8 Tm
0 g
0 G
[( )-2( )-2( )-2( )-2( )-2( )10( )] TJ
ET)";

static void BM_Contents_ParseContentStream(benchmark::State& state) {
	FileHandle* test_file = nullptr;
	InputOutputStreamHandle* file_stream = nullptr;

	InputOutputStreamHandle* content_io_stream = nullptr;
	InputStreamHandle* content_input_stream = nullptr;

	InputOutputStream_CreateFromMemory(&file_stream);
	File_OpenStream(file_stream, "temp", &test_file);

	InputOutputStream_CreateFromMemory(&content_io_stream);
	InputOutputStream_WriteString(content_io_stream, SAMPLE_CONTENT_STREAM);
	InputOutputStream_ToInputStream(content_io_stream, &content_input_stream);

	for (auto _ : state) {
		ContentParserHandle* content_parser = nullptr;
		ContentInstructionCollectionHandle* content_instruction_collection = nullptr;

		InputStream_SetInputPosition(content_input_stream, 0);

		ContentParser_Create(test_file, content_input_stream, &content_parser);
		ContentParser_ReadInstructionCollection(content_parser, &content_instruction_collection);

		ContentInstructionCollection_Release(content_instruction_collection);
		ContentParser_Release(content_parser);
	}

	// Cleanup
	InputStream_Release(content_input_stream);
	InputOutputStream_Release(content_io_stream);

	File_Release(test_file);
	InputOutputStream_Release(file_stream);
}

BENCHMARK(BM_Contents_ParseContentStream);

static const char MINIMALIST_DOCUMENT[] = R"(%PDF-1.7
1 0 obj
<</Pages 2 0 R /Type /Catalog>>
endobj
2 0 obj
<</Count 0 /Type /Pages>>
endobj
3 0 obj
<</CreationDate (D:20170831161944Z) /Producer (I am the producer)>>
endobj

xref
0 4
0000000000 65535 f 
0000000009 00000 n 
0000000056 00000 n 
0000000097 00000 n 
trailer
<</Info 3 0 R /Root 1 0 R /Size 4>>
startxref
181
%%EOF
)";

static void BM_FileSaveParse(benchmark::State& state) {
	for (auto _ : state) {
		FileHandle* test_file = nullptr;
		InputOutputStreamHandle* io_stream = nullptr;

		InputOutputStream_CreateFromMemory(&io_stream);
		InputOutputStream_WriteString(io_stream, MINIMALIST_DOCUMENT);

		File_OpenStream(io_stream, "temp", &test_file);
		File_Initialize(test_file);
		File_Release(test_file);

		// Cleanup
		InputOutputStream_Release(io_stream);
	}
}

BENCHMARK(BM_FileSaveParse);

// Run the benchmark
BENCHMARK_MAIN();
