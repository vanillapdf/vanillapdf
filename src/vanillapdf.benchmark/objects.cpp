#include "benchmark.h"

template <class ...Args>
static void BM_RealObjectToPdf(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    real_type value = std::get<0>(args_tuple);
    integer_type precision = std::get<1>(args_tuple);

    for (auto _ : state) {
        RealObjectHandle* real_object = nullptr;
        ObjectHandle* base_object = nullptr;
        BufferHandle* object_pdf_buffer = nullptr;

        // Create data from defined value
        RealObject_CreateFromData(value, precision, &real_object);

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

    const char* encoded_hex_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        HexadecimalStringObjectHandle* hex_string_object = nullptr;
        StringObjectHandle* string_object = nullptr;
        ObjectHandle* base_object = nullptr;
        BufferHandle* object_pdf_buffer = nullptr;

        // Create data from defined value
        HexadecimalStringObject_CreateFromEncodedString(encoded_hex_string, &hex_string_object);

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

    const char* encoded_literal_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        LiteralStringObjectHandle* literal_string_object = nullptr;
        StringObjectHandle* string_object = nullptr;
        ObjectHandle* base_object = nullptr;
        BufferHandle* object_pdf_buffer = nullptr;

        // Create data from defined value
        LiteralStringObject_CreateFromEncodedString(encoded_literal_string, &literal_string_object);

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
BENCHMARK_CAPTURE(BM_LiteralStringObjectToPdf, string_octal, "\\001\\002\\003\\004\\252\\253\\254\\255");

template <class ...Args>
static void BM_NameObjectToPdf(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_name = std::get<0>(args_tuple);

    for (auto _ : state) {
        NameObjectHandle* name_object = nullptr;
        ObjectHandle* base_object = nullptr;
        BufferHandle* object_pdf_buffer = nullptr;

        // Create data from defined value
        NameObject_CreateFromEncodedString(encoded_name, &name_object);

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

// ----- String_GetValue -----

template <class ...Args>
static void BM_StringGetValue_Literal(benchmark::State& state, Args&&... args) {
    auto args_tuple = std::make_tuple(std::move(args)...);

    const char* encoded_string = std::get<0>(args_tuple);

    for (auto _ : state) {
        LiteralStringObjectHandle* string_object = nullptr;
        BufferHandle* value_buffer = nullptr;

        // Create data from defined value
        LiteralStringObject_CreateFromEncodedString(encoded_string, &string_object);

        // Get base object as it is the only one having ToPdf exposed
        LiteralStringObject_GetValue(string_object, &value_buffer);

        // Cleanup
        Buffer_Release(value_buffer);
        LiteralStringObject_Release(string_object);
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
        HexadecimalStringObjectHandle* string_object = nullptr;
        BufferHandle* value_buffer = nullptr;

        // Create data from defined value
        HexadecimalStringObject_CreateFromEncodedString(encoded_string, &string_object);

        // Get base object as it is the only one having ToPdf exposed
        HexadecimalStringObject_GetValue(string_object, &value_buffer);

        // Cleanup
        Buffer_Release(value_buffer);
        HexadecimalStringObject_Release(string_object);
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
        HexadecimalStringObjectHandle* string_object = nullptr;

        // Create data from defined value
        HexadecimalStringObject_CreateFromEncodedString(encoded_string, &string_object);

        // Cleanup
        HexadecimalStringObject_Release(string_object);
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
        LiteralStringObjectHandle* string_object = nullptr;

        // Create data from defined value
        LiteralStringObject_CreateFromEncodedString(encoded_string, &string_object);

        // Cleanup
        LiteralStringObject_Release(string_object);
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
        NameObjectHandle* name_object = nullptr;

        // Create data from defined value
        NameObject_CreateFromEncodedString(encoded_string, &name_object);

        // Cleanup
        NameObject_Release(name_object);
    }
}

BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Name, string_empty, "");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Name, string_basic, "abcdefghijklmnopqrstuvwxyz");
BENCHMARK_CAPTURE(BM_CreateFromEncodedString_Name, string_hexadecimal, "#01#02#03#FA#FB#FC#FD#FE#FF");

// ----- Object Creation/Destruction Benchmarks -----
// Measures per-object allocation cost including mutex and member construction/destruction

static void BM_IntegerObject_Create(benchmark::State& state) {
    for (auto _ : state) {
        IntegerObjectHandle* obj = nullptr;
        IntegerObject_Create(&obj);
        IntegerObject_Release(obj);
    }
}

BENCHMARK(BM_IntegerObject_Create);

static void BM_RealObject_Create(benchmark::State& state) {
    for (auto _ : state) {
        RealObjectHandle* obj = nullptr;
        RealObject_Create(&obj);
        RealObject_Release(obj);
    }
}

BENCHMARK(BM_RealObject_Create);

// ----- Bulk Object Allocation Benchmarks -----
// Measures throughput and memory pressure when creating many objects

static void BM_IntegerObject_BulkCreate(benchmark::State& state) {
    const auto count = state.range(0);

    for (auto _ : state) {
        std::vector<IntegerObjectHandle*> objects(count, nullptr);

        for (int64_t i = 0; i < count; ++i) {
            IntegerObject_CreateFromIntegerValue(i, &objects[i]);
        }

        for (int64_t i = 0; i < count; ++i) {
            IntegerObject_Release(objects[i]);
        }
    }

    state.SetItemsProcessed(state.iterations() * count);
}

BENCHMARK(BM_IntegerObject_BulkCreate)->Range(64, 1 << 16);

static void BM_RealObject_BulkCreate(benchmark::State& state) {
    const auto count = state.range(0);

    for (auto _ : state) {
        std::vector<RealObjectHandle*> objects(count, nullptr);

        for (int64_t i = 0; i < count; ++i) {
            RealObject_CreateFromData(static_cast<real_type>(i) * 0.5, 2, &objects[i]);
        }

        for (int64_t i = 0; i < count; ++i) {
            RealObject_Release(objects[i]);
        }
    }

    state.SetItemsProcessed(state.iterations() * count);
}

BENCHMARK(BM_RealObject_BulkCreate)->Range(64, 1 << 16);
