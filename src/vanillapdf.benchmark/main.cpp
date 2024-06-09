#include "benchmark.h"

static void BM_RealObjectToPdf(benchmark::State& state) {
	for (auto _ : state) {
		RealObjectHandle* real_object;
		ObjectHandle* base_object;
		BufferHandle* object_pdf_buffer;

		const real_type REAL_VALUE_DATA = 752.43f;
		const integer_type REAL_VALUE_DATA_PRECISION = 2;

		// Create data from defined value
		RealObject_CreateFromData(REAL_VALUE_DATA, REAL_VALUE_DATA_PRECISION, &real_object);

		RealObject_SetValue(real_object, REAL_VALUE_DATA);
		RealObject_ToObject(real_object, &base_object);

		// Get PDF representation of RealObject value
		Object_ToPdf(base_object, &object_pdf_buffer);

		// Cleanup
		Buffer_Release(object_pdf_buffer);
		Object_Release(base_object);
		RealObject_Release(real_object);
	}
}

BENCHMARK(BM_RealObjectToPdf)
	->Args({ 752, 0 })
	->Args({ 752, 2 });

// Run the benchmark
BENCHMARK_MAIN();
