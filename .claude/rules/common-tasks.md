# Common Tasks

## Adding New PDF Object Type

1. Create C++ class in `src/vanillapdf/syntax/objects/` (e.g., `new_object.h`, `new_object.cpp`)
2. Add parser logic in `src/vanillapdf/syntax/parsers/`
3. Create C API header in `include/vanillapdf/syntax/c_new_object.h`
4. Implement C interface wrapper in `src/vanillapdf/implementation/syntax/c_new_object.cpp`
5. Add to main API header `include/vanillapdf/c_vanillapdf_api.h`
6. Add unit tests in `src/vanillapdf.unittest/objects_test.cpp`

## Adding Content Stream Operation

1. Define operation in `src/vanillapdf/contents/content_stream_operations.h`
2. Implement in `src/vanillapdf/contents/content_stream_operations.cpp`
3. Add parser support in `src/vanillapdf/contents/content_stream_parser.h`
4. Create C API header in `include/vanillapdf/contents/c_content_operation.h` (if needed)
5. Create C interface wrapper in `src/vanillapdf/implementation/contents/`
6. Add integration tests in `src/vanillapdf.test/`

## Adding New Semantic Object

1. Create C++ class in `src/vanillapdf/semantics/objects/` (e.g., `new_semantic.h`, `new_semantic.cpp`)
2. Inherit from `HighLevelObject` base class
3. Create C API header in `include/vanillapdf/semantics/c_new_semantic.h`
4. Implement C interface wrapper in `src/vanillapdf/implementation/semantics/`
5. Add to main API header `include/vanillapdf/c_vanillapdf_api.h`
6. Add tests in appropriate test directory

## Signature Verification

**Components** (in `src/vanillapdf/utils/`):
- `SignatureVerifier` - PKCS#7 signature verification (raw bytes)
- `SignatureVerificationResult` - Verification status and certificate chain
- `TrustedCertificateStore` - Certificate store for chain validation
- `DigitalSignatureExtensions::Verify` - Document-level verification

**Testing**:
```bash
ctest --preset windows-x64-msvc-17 --build-config Debug \
  -R "TrustedCertificateStore|SignatureVerifier|SignatureVerificationResult" \
  --output-on-failure
```

## Debugging and Development

- Use sanitizers in Debug builds: `-DVANILLAPDF_ENABLE_ASAN=ON`, `-DVANILLAPDF_ENABLE_UBSAN=ON`, or `-DVANILLAPDF_ENABLE_TSAN=ON` (ASan and TSan are mutually exclusive)
- Visual Studio .natvis files available for debugging C++ objects (`public.natvis`, `vanillapdf.natvis`)
- Precompiled headers are used (`precompiled.h`) for faster builds
- C API headers use handle-based system (opaque pointers) for ABI stability
