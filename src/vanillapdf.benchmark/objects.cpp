#include "benchmark.h"
#include "handle_guard.h"

template <class ...Args>
static void BM_RealObjectToPdf(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    real_type value = std::get<0>(args_tuple);
    integer_type precision = std::get<1>(args_tuple);

    for (auto _ : state) {
        HandleGuard<RealObjectHandle, RealObject_Release> real_object;
        HandleGuard<ObjectHandle, Object_Release> base_object;
        HandleGuard<BufferHandle, Buffer_Release> object_pdf_buffer;

        // Create data from defined value
        RealObject_CreateFromData(value, precision, real_object.out());

        // Convert to base object
        RealObject_ToObject(real_object, base_object.out());

        // Get PDF representation of RealObject value
        Object_ToPdf(base_object, object_pdf_buffer.out());
    }
}

BENCHMARK_CAPTURE(BM_RealObjectToPdf, no_precision, 752.43f, 0);
BENCHMARK_CAPTURE(BM_RealObjectToPdf, int_precision, 752.43f, 2);

template <class ...Args>
static void BM_HexadecimalStringObjectToPdf(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_hex_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> hex_string_object;
        HandleGuard<StringObjectHandle, StringObject_Release> string_object;
        HandleGuard<ObjectHandle, Object_Release> base_object;
        HandleGuard<BufferHandle, Buffer_Release> object_pdf_buffer;

        // Create data from defined value
        HexadecimalStringObject_CreateFromEncodedString(encoded_hex_string, hex_string_object.out());

        // Get base object as it is the only one having ToPdf exposed
        HexadecimalStringObject_ToStringObject(hex_string_object, string_object.out());
        StringObject_ToObject(string_object, base_object.out());

        // Get PDF representation of HexadecimalStringObject value
        Object_ToPdf(base_object, object_pdf_buffer.out());
    }
}

BENCHMARK_CAPTURE(BM_HexadecimalStringObjectToPdf, string_empty, "");
BENCHMARK_CAPTURE(BM_HexadecimalStringObjectToPdf, string_unpaired, "F");
BENCHMARK_CAPTURE(BM_HexadecimalStringObjectToPdf, string_values, "000A0FF0FF");

template <class ...Args>
static void BM_LiteralStringObjectToPdf(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_literal_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> literal_string_object;
        HandleGuard<StringObjectHandle, StringObject_Release> string_object;
        HandleGuard<ObjectHandle, Object_Release> base_object;
        HandleGuard<BufferHandle, Buffer_Release> object_pdf_buffer;

        // Create data from defined value
        LiteralStringObject_CreateFromEncodedString(encoded_literal_string, literal_string_object.out());

        // Get base object as it is the only one having ToPdf exposed
        LiteralStringObject_ToStringObject(literal_string_object, string_object.out());
        StringObject_ToObject(string_object, base_object.out());

        // Get PDF representation of LiteralStringObject value
        Object_ToPdf(base_object, object_pdf_buffer.out());
    }
}

BENCHMARK_CAPTURE(BM_LiteralStringObjectToPdf, string_empty, "");
BENCHMARK_CAPTURE(BM_LiteralStringObjectToPdf, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_LiteralStringObjectToPdf, string_octal, "\\001\\002\\003\\004\\252\\253\\254\\255");

template <class ...Args>
static void BM_NameObjectToPdf(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_name = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<NameObjectHandle, NameObject_Release> name_object;
        HandleGuard<ObjectHandle, Object_Release> base_object;
        HandleGuard<BufferHandle, Buffer_Release> object_pdf_buffer;

        // Create data from defined value
        NameObject_CreateFromEncodedString(encoded_name, name_object.out());

        // Get base object as it is the only one having ToPdf exposed
        NameObject_ToObject(name_object, base_object.out());

        // Get PDF representation of LiteralStringObject value
        Object_ToPdf(base_object, object_pdf_buffer.out());
    }
}

BENCHMARK_CAPTURE(BM_NameObjectToPdf, string_empty, "");
BENCHMARK_CAPTURE(BM_NameObjectToPdf, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_NameObjectToPdf, string_hexadecimal, "#01#02#03#FA#FB#FC#FD#FE#FF");

// ----- String_GetValue -----

template <class ...Args>
static void BM_StringGetValue_Literal(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> string_object;
        HandleGuard<BufferHandle, Buffer_Release> value_buffer;

        // Create data from defined value
        LiteralStringObject_CreateFromEncodedString(encoded_string, string_object.out());

        // Get base object as it is the only one having ToPdf exposed
        LiteralStringObject_GetValue(string_object, value_buffer.out());
    }
}

BENCHMARK_CAPTURE(BM_StringGetValue_Literal, string_empty, "");
BENCHMARK_CAPTURE(BM_StringGetValue_Literal, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_StringGetValue_Literal, string_octal, "\\001\\002\\003\\004\\252\\253\\254\\255");

template <class ...Args>
static void BM_StringGetValue_Hexadecimal(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> string_object;
        HandleGuard<BufferHandle, Buffer_Release> value_buffer;

        // Create data from defined value
        HexadecimalStringObject_CreateFromEncodedString(encoded_string, string_object.out());

        // Get base object as it is the only one having ToPdf exposed
        HexadecimalStringObject_GetValue(string_object, value_buffer.out());
    }
}

BENCHMARK_CAPTURE(BM_StringGetValue_Hexadecimal, string_empty, "");
BENCHMARK_CAPTURE(BM_StringGetValue_Hexadecimal, string_unpaired, "F");
BENCHMARK_CAPTURE(BM_StringGetValue_Hexadecimal, string_values, "000A0FF0FF");

template <class ...Args>
static void BM_CreateFromEncodedString_Hexadecimal(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> string_object;

        // Create data from defined value
        HexadecimalStringObject_CreateFromEncodedString(encoded_string, string_object.out());
    }
}

BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Hexadecimal, string_empty, "");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Hexadecimal, string_unpaired, "F");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Hexadecimal, string_values, "000A0FF0FF");

template <class ...Args>
static void BM_CreateFromEncodedString_Literal(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> string_object;

        // Create data from defined value
        LiteralStringObject_CreateFromEncodedString(encoded_string, string_object.out());
    }
}

BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Literal, string_empty, "");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Literal, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Literal, string_octal, "\\001\\002\\003\\004\\252\\253\\254\\255");

template <class ...Args>
static void BM_CreateFromEncodedString_Name(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HandleGuard<NameObjectHandle, NameObject_Release> name_object;

        // Create data from defined value
        NameObject_CreateFromEncodedString(encoded_string, name_object.out());
    }
}

BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Name, string_empty, "");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Name, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Name, string_hexadecimal, "#01#02#03#FA#FB#FC#FD#FE#FF");
