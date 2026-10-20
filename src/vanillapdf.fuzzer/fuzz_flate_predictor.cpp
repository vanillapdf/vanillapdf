#include "fuzzer.h"
#include "handle_guard.h"

#include <fuzzer/FuzzedDataProvider.h>

#include <cstddef>
#include <cstdint>
#include <vector>

// FlateDecode with /DecodeParms. The sibling flate_decode target always decodes
// without parameters, so it never reaches the predictor at all - and the
// predictor geometry is the interesting part, because /Colors, /Columns and
// /BitsPerComponent come straight out of the document and decide both the size
// of the scanline buffers and the stride the PNG filters walk them with.
//
// FuzzedDataProvider takes integrals from the end of the input and the
// remaining bytes from the front, so the deflate stream stays contiguous at
// offset 0 and a plain deflate file still works as a seed.
// https://github.com/google/fuzzing/blob/master/docs/split-inputs.md

// Inserts a /DecodeParms integer entry. A failure here only means this input
// exercises fewer parameters, which is not worth abandoning the run over.
static void insert_integer(DictionaryObjectHandle* dictionary, string_type key, bigint_type value) {
    HandleGuard<NameObjectHandle, NameObject_Release> name;
    if (NameObject_CreateFromDecodedString(key, name.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return;
    }

    HandleGuard<IntegerObjectHandle, IntegerObject_Release> integer;
    if (IntegerObject_CreateFromIntegerValue(value, integer.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return;
    }

    HandleGuard<ObjectHandle, Object_Release> object;
    if (IntegerObject_ToObject(integer, object.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return;
    }

    DictionaryObject_Insert(dictionary, name, object, VANILLAPDF_RV_TRUE);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static bool initialized = (fuzzer_init(), true);
    (void)initialized;

    FuzzedDataProvider provider(data, size);

    // Consumed from the back, in this order. Exhausted input yields zero, which
    // is itself a geometry worth exercising.
    auto predictor = provider.ConsumeIntegral<uint8_t>();
    auto colors = provider.ConsumeIntegral<uint8_t>();
    auto bits_per_component = provider.ConsumeIntegral<uint8_t>();
    auto columns = provider.ConsumeIntegral<uint8_t>();

    // Consumed from the front - whatever is left is the deflate stream.
    std::vector<uint8_t> stream = provider.ConsumeRemainingBytes<uint8_t>();

    HandleGuard<BufferHandle, Buffer_Release> input;
    if (Buffer_CreateFromData(reinterpret_cast<const char*>(stream.data()),
                              static_cast<size_type>(stream.size()),
                              input.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> parameters;
    if (DictionaryObject_Create(parameters.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    insert_integer(parameters, "Predictor", predictor);
    insert_integer(parameters, "Colors", colors);
    insert_integer(parameters, "BitsPerComponent", bits_per_component);
    insert_integer(parameters, "Columns", columns);

    HandleGuard<FlateDecodeFilterHandle, FlateDecodeFilter_Release> filter;
    if (FlateDecodeFilter_Create(filter.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<BufferHandle, Buffer_Release> output;
    FlateDecodeFilter_DecodeParams(filter, input, parameters, output.out());

    return 0;
}
