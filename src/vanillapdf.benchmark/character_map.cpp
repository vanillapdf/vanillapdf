#include "benchmark.h"
#include "handle_guard.h"

// Benchmark BaseFontRange_Contains: checks if a key is within a range
// using ValueEqualLessThan byte-by-byte comparison
static void BM_BaseFontRange_Contains(benchmark::State& state) {
    // Setup: range <0020> to <007E> (printable ASCII)
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    BaseFontRange_Create(range.out());

    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> low;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> high;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> dest;

    HexadecimalStringObject_CreateFromEncodedString("0020", low.out());
    HexadecimalStringObject_CreateFromEncodedString("007E", high.out());
    HexadecimalStringObject_CreateFromEncodedString("0020", dest.out());

    BaseFontRange_SetRangeLow(range, low);
    BaseFontRange_SetRangeHigh(range, high);

    HandleGuard<StringObjectHandle, StringObject_Release> dest_str;
    HandleGuard<ObjectHandle, Object_Release> dest_obj;
    HexadecimalStringObject_ToStringObject(dest, dest_str.out());
    StringObject_ToObject(dest_str, dest_obj.out());
    BaseFontRange_SetDestination(range, dest_obj);

    // Create key buffer <0041> ('A', middle of range)
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    HexadecimalStringObject_CreateFromEncodedString("0041", key_hex.out());
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    HexadecimalStringObject_GetValue(key_hex, key_buf.out());

    for (auto _ : state) {
        boolean_type result = VANILLAPDF_RV_FALSE;
        BaseFontRange_Contains(range, key_buf, &result);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_BaseFontRange_Contains);

// Benchmark BaseFontRange_GetMappedValue: computes dest + (key - low)
// using custom Difference (bit-by-bit subtraction) and Increment (byte-by-byte addition)
static void BM_BaseFontRange_GetMappedValue(benchmark::State& state) {
    // Setup: range <0020> to <007E> with destination <0020>
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    BaseFontRange_Create(range.out());

    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> low;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> high;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> dest;

    HexadecimalStringObject_CreateFromEncodedString("0020", low.out());
    HexadecimalStringObject_CreateFromEncodedString("007E", high.out());
    HexadecimalStringObject_CreateFromEncodedString("0020", dest.out());

    BaseFontRange_SetRangeLow(range, low);
    BaseFontRange_SetRangeHigh(range, high);

    HandleGuard<StringObjectHandle, StringObject_Release> dest_str;
    HandleGuard<ObjectHandle, Object_Release> dest_obj;
    HexadecimalStringObject_ToStringObject(dest, dest_str.out());
    StringObject_ToObject(dest_str, dest_obj.out());
    BaseFontRange_SetDestination(range, dest_obj);

    // Create key buffer <0041> ('A', middle of range)
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    HexadecimalStringObject_CreateFromEncodedString("0041", key_hex.out());
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    HexadecimalStringObject_GetValue(key_hex, key_buf.out());

    for (auto _ : state) {
        HandleGuard<BufferHandle, Buffer_Release> result;
        BaseFontRange_GetMappedValue(range, key_buf, result.out());
        benchmark::DoNotOptimize(result.get());
    }
}

BENCHMARK(BM_BaseFontRange_GetMappedValue);

// Benchmark with 4-byte CMap codes (CJK font scenario)
static void BM_BaseFontRange_GetMappedValue_4byte(benchmark::State& state) {
    // Setup: range <00004E00> to <00009FFF> (CJK Unified Ideographs block)
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    BaseFontRange_Create(range.out());

    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> low;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> high;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> dest;

    HexadecimalStringObject_CreateFromEncodedString("00004E00", low.out());
    HexadecimalStringObject_CreateFromEncodedString("00009FFF", high.out());
    HexadecimalStringObject_CreateFromEncodedString("00004E00", dest.out());

    BaseFontRange_SetRangeLow(range, low);
    BaseFontRange_SetRangeHigh(range, high);

    HandleGuard<StringObjectHandle, StringObject_Release> dest_str;
    HandleGuard<ObjectHandle, Object_Release> dest_obj;
    HexadecimalStringObject_ToStringObject(dest, dest_str.out());
    StringObject_ToObject(dest_str, dest_obj.out());
    BaseFontRange_SetDestination(range, dest_obj);

    // Key in the middle of CJK range
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    HexadecimalStringObject_CreateFromEncodedString("00006F22", key_hex.out());
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    HexadecimalStringObject_GetValue(key_hex, key_buf.out());

    for (auto _ : state) {
        HandleGuard<BufferHandle, Buffer_Release> result;
        BaseFontRange_GetMappedValue(range, key_buf, result.out());
        benchmark::DoNotOptimize(result.get());
    }
}

BENCHMARK(BM_BaseFontRange_GetMappedValue_4byte);
