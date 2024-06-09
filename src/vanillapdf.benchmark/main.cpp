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

// Run the benchmark
BENCHMARK_MAIN();
