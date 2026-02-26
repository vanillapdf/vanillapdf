---
paths:
  - "src/vanillapdf.unittest/**"
  - "src/vanillapdf.test/**"
  - "src/vanillapdf.benchmark/**"
---

# Testing

## Test Organization

- Unit tests in `src/vanillapdf.unittest/`
- Integration tests in `src/vanillapdf.test/`
- Benchmarks in `src/vanillapdf.benchmark/`

## Running Tests

Enable tests with `-DVANILLAPDF_ENABLE_TESTS=ON` (enabled by default) and run:
```bash
ctest --preset windows-x64-msvc-18
```

Run specific test suites:
```bash
ctest --preset windows-x64-msvc-18 -R "unittest"     # Unit tests only
ctest --preset windows-x64-msvc-18 -R "test"         # Integration tests only
ctest --preset windows-x64-msvc-18 -R "benchmark"    # Benchmarks only
```

Run individual test cases with verbose output:
```bash
ctest --preset windows-x64-msvc-18 -R "objects_test" --verbose
ctest --preset windows-x64-msvc-18 -R "signature_verifier_test" --output-on-failure
```

**Windows (Visual Studio) specific**: Visual Studio is a multi-config generator, so you must specify build configuration:
```bash
# Specify configuration with --build-config
ctest --preset windows-x64-msvc-18 --build-config Debug -R "SignatureVerifier" --output-on-failure
ctest --preset windows-x64-msvc-18 --build-config Release -R "unittest"
```

## Code Coverage

Enable code coverage (GCC/Clang only):
```bash
cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build --preset linux-x64-gcc
ctest --preset linux-x64-gcc
gcovr -r . --xml-pretty -o coverage.xml  # Generate coverage report
```

Coverage reports are automatically generated in CI and uploaded to Codecov.

## Sanitizers

Available sanitizers (Linux/macOS GCC/Clang only, Debug builds):
```bash
cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_ASAN=ON -DCMAKE_BUILD_TYPE=Debug
cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_UBSAN=ON -DCMAKE_BUILD_TYPE=Debug
cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_TSAN=ON -DCMAKE_BUILD_TYPE=Debug
```

Note: ASan and TSan are mutually exclusive and cannot be enabled simultaneously.

## Performance Improvement Workflow

Every performance change must be justified with benchmark data:

1. **Add benchmark** — write a Google Benchmark test covering the hot path before making changes
2. **Measure baseline** — build and run in Release mode, record results
3. **Commit baseline** — commit the benchmark with baseline results in a comment
4. **Implement the change**
5. **Measure impact** — run the same benchmark in Release mode, record new results
6. **Commit with comparison** — include both before/after results in the benchmark comment
