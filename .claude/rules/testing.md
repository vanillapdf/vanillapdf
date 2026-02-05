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
ctest --preset windows-x64-msvc-17
```

Run specific test suites:
```bash
ctest --preset windows-x64-msvc-17 -R "unittest"     # Unit tests only
ctest --preset windows-x64-msvc-17 -R "test"         # Integration tests only
ctest --preset windows-x64-msvc-17 -R "benchmark"    # Benchmarks only
```

Run individual test cases with verbose output:
```bash
ctest --preset windows-x64-msvc-17 -R "objects_test" --verbose
ctest --preset windows-x64-msvc-17 -R "signature_verifier_test" --output-on-failure
```

**Windows (Visual Studio) specific**: Visual Studio is a multi-config generator, so you must specify build configuration:
```bash
# Specify configuration with --build-config
ctest --preset windows-x64-msvc-17 --build-config Debug -R "SignatureVerifier" --output-on-failure
ctest --preset windows-x64-msvc-17 --build-config Release -R "unittest"
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

Use address sanitizer for memory safety testing:
```bash
cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_STACK_SANITIZER=ON -DCMAKE_BUILD_TYPE=Debug
```
