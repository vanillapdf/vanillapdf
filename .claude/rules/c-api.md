---
paths:
  - "src/vanillapdf/implementation/**"
  - "include/vanillapdf/**"
---

# C API Development

## Core Type System (c_types.h)

| Type | C Type | Purpose |
|------|--------|---------|
| `error_type` | `uint32_t` | Return value for all functions |
| `boolean_type` | `int8_t` | Boolean (`VANILLAPDF_RV_TRUE` / `VANILLAPDF_RV_FALSE`) |
| `string_type` | `const char*` | Null-terminated UTF-8 string |
| `byte_array_type` | `char*` | Writeable byte array |
| `real_type` | `double` | Floating-point |
| `ushort_type` | `uint16_t` | 16-bit unsigned |
| `integer_type` | `int32_t` | 32-bit signed |
| `offset_type` | `int64_t` | File position |
| `size_type` | `uint32_t` (32-bit) / `uint64_t` (64-bit) | Size/count |
| `bigint_type` | `int64_t` | 64-bit signed |
| `biguint_type` | `uint64_t` | 64-bit unsigned |

## Error Codes (c_values.h)

**Success:**
- `VANILLAPDF_ERROR_SUCCESS`

**General:**
- `VANILLAPDF_ERROR_PARAMETER_VALUE` - Invalid parameter
- `VANILLAPDF_ERROR_NOT_SUPPORTED` - Unsupported operation
- `VANILLAPDF_ERROR_USER_CANCELLED` - User cancelled
- `VANILLAPDF_ERROR_GENERAL` - Unknown error (check logging)
- `VANILLAPDF_ERROR_INSUFFICIENT_SPACE` - Buffer too small
- `VANILLAPDF_ERROR_ZLIB_DATA` - Corrupted compressed data

**License:**
- `VANILLAPDF_ERROR_INVALID_LICENSE`, `VANILLAPDF_ERROR_LICENSE_REQUIRED`

**Syntax:**
- `VANILLAPDF_ERROR_CONVERSION` - Invalid object type cast
- `VANILLAPDF_ERROR_FILE_DISPOSED` - Handle already disposed
- `VANILLAPDF_ERROR_FILE_NOT_INITIALIZED` - Call `File_Initialize` first
- `VANILLAPDF_ERROR_OBJECT_MISSING` - Required object not found
- `VANILLAPDF_ERROR_PARSE_EXCEPTION` - Damaged PDF / parsing error
- `VANILLAPDF_ERROR_INVALID_PASSWORD` - Wrong password/key
- `VANILLAPDF_ERROR_DUPLICATE_KEY` - Key exists in dictionary

**Semantic:**
- `VANILLAPDF_ERROR_OPTIONAL_ENTRY_MISSING` - Optional entry absent
- `VANILLAPDF_ERROR_SEMANTIC_CONTEXT` - Unexpected object type

## Thread-Local Error Retrieval (c_errors.h)

```c
Errors_GetLastError(error_type* result);
Errors_GetPrintableErrorTextLength(error_type value, size_type* size); // includes null
Errors_GetPrintableErrorText(error_type value, char* data, size_type size);
Errors_GetLastErrorMessageLength(size_type* result);
Errors_GetLastErrorMessage(char* data, size_type size);
```

Pattern: call `_Length` first, allocate, then call with buffer.

## Handle System (c_handles.h)

All handles are opaque pointers via `DECLARE_OBJECT_HANDLE(X)` → creates `XHandle` type.

### All Handle Types by Category

**Base:**
- `IUnknownHandle` - Reference-counted base for all handles

**Syntax - Objects:**
- `ObjectHandle`, `ArrayObjectHandle`, `BooleanObjectHandle`, `NameObjectHandle`, `NullObjectHandle`
- `RealObjectHandle`, `IntegerObjectHandle`, `StreamObjectHandle`
- `StringObjectHandle`, `HexadecimalStringObjectHandle`, `LiteralStringObjectHandle`
- `IndirectReferenceObjectHandle`, `DictionaryObjectHandle`, `DictionaryObjectIteratorHandle`

**Syntax - Attributes:**
- `BaseObjectAttributeHandle`, `ImageMetadataObjectAttributeHandle`
- `SerializationOverrideObjectAttributeHandle`, `ObjectAttributeListHandle`

**Syntax - File:**
- `FileHandle`, `FileWriterHandle`, `FileWriterObserverHandle`

**Syntax - Xref:**
- `XrefHandle`, `XrefIteratorHandle`, `XrefEntryHandle`
- `XrefFreeEntryHandle`, `XrefUsedEntryHandle`, `XrefCompressedEntryHandle`
- `XrefChainHandle`, `XrefChainIteratorHandle`

**Syntax - Filters:**
- `FilterBaseHandle`, `FlateDecodeFilterHandle`, `DCTDecodeFilterHandle`
- `ASCII85DecodeFilterHandle`, `ASCIIHexDecodeFilterHandle`
- `LZWDecodeFilterHandle`, `JPXDecodeFilterHandle`

**Contents - Instructions:**
- `ContentObjectHandle`, `ContentOperationHandle`, `ContentInstructionHandle`
- `ContentInstructionCollectionHandle`, `ContentInstructionCollectionIteratorHandle`

**Contents - Operations:**
- `ContentOperationGenericHandle`, `ContentOperationTextFontHandle`
- `ContentOperationTextShowHandle`, `ContentOperationTextShowArrayHandle`
- `ContentOperationBeginTextHandle`, `ContentOperationEndTextHandle`

**Contents - Other:**
- `ContentOperatorHandle`, `ContentOperatorUnknownHandle`
- `ContentObjectTextHandle`, `ContentObjectInlineImageHandle`
- `ContentParserHandle`

**Semantics - Document:**
- `DocumentHandle`, `CatalogHandle`, `DocumentInfoHandle`
- `PageTreeHandle`, `PageObjectHandle`, `PageContentsHandle`
- `ResourceDictionaryHandle`

**Semantics - Extensions/Labels:**
- `DeveloperExtensionsHandle`, `DeveloperExtensionsIteratorHandle`, `DeveloperExtensionHandle`
- `PageLabelsHandle`, `PageLabelHandle`

**Semantics - Navigation:**
- `OutlineBaseHandle`, `OutlineHandle`, `OutlineItemHandle`
- `OutlineItemColorHandle`, `OutlineItemFlagsHandle`

**Semantics - Destinations:**
- `DestinationNameTreeHandle`, `DestinationNameTreeIteratorHandle`
- `NamedDestinationsHandle`, `DestinationHandle` (base)
- `XYZDestinationHandle`, `FitDestinationHandle`, `FitHorizontalDestinationHandle`
- `FitVerticalDestinationHandle`, `FitRectangleDestinationHandle`
- `FitBoundingBoxDestinationHandle`, `FitBoundingBoxHorizontalDestinationHandle`
- `FitBoundingBoxVerticalDestinationHandle`

**Semantics - Names:**
- `NameDictionaryHandle`

**Semantics - Annotations:**
- `PageAnnotationsHandle`, `AnnotationHandle`, `LinkAnnotationHandle`

**Semantics - Forms:**
- `InteractiveFormHandle`, `FieldCollectionHandle`, `FieldHandle`
- `ButtonFieldHandle`, `TextFieldHandle`, `ChoiceFieldHandle`, `SignatureFieldHandle`

**Semantics - Signatures:**
- `DigitalSignatureHandle`, `ByteRangeCollectionHandle`, `ByteRangeHandle`
- `DocumentSignerHandle`, `DocumentSignatureSettingsHandle`

**Semantics - Encryption:**
- `DocumentEncryptionSettingsHandle`

**Semantics - Fonts:**
- `FontHandle`, `Type0FontHandle`, `FontMapHandle`
- `CharacterMapHandle`, `EmbeddedCharacterMapHandle`, `UnicodeCharacterMapHandle`
- `BaseFontRangeHandle`

**Semantics - Other:**
- `ViewerPreferencesHandle`, `PageRangeHandle`, `PageSubRangeHandle`
- `DateHandle`, `RectangleHandle`

**Utilities:**
- `BufferHandle`, `BufferArrayHandle`
- `SigningKeyHandle`, `EncryptionKeyHandle`, `PKCS12KeyHandle`
- `TrustedCertificateStoreHandle`
- `SignatureVerificationResultHandle`, `SignatureVerificationSettingsHandle`
- `InputStreamHandle`, `OutputStreamHandle`
- `MemoryBufferOutputStreamHandle`, `InputOutputStreamHandle`

## Memory Management & Reference Counting

### IUnknown Interface (c_unknown_interface.h)

```c
IUnknown_AddRef(IUnknownHandle* handle);    // Increment ref count
IUnknown_Release(IUnknownHandle* handle);   // Decrement; delete when 0
```

Every handle type also has `XxxHandle_Release()` wrapper.

### Ownership Rules

- **Create/Open functions**: Caller owns returned handle → must call `*_Release()`
- **Get accessor functions**: Caller owns returned handle → must call `*_Release()`
- **ToUnknown/FromUnknown**: Caller owns result → must call `*_Release()`
- **Function parameters**: Caller retains ownership of passed handles

### Typical Usage Pattern

```c
DocumentHandle* doc = NULL;
error_type err = Document_Open("file.pdf", &doc);
if (err == VANILLAPDF_ERROR_SUCCESS && doc != NULL) {
    // Use doc...
    Document_Release(doc);  // MUST call
}
```

### Type Conversion

```c
Xxx_ToUnknown(XxxHandle* handle, IUnknownHandle** result);   // Upcast (increments ref)
Xxx_FromUnknown(IUnknownHandle* handle, XxxHandle** result);  // Downcast
Xxx_ToObject(XxxHandle* handle, ObjectHandle** result);       // To base Object
Xxx_FromObject(ObjectHandle* handle, XxxHandle** result);     // From base Object
```

## Enums

### ObjectType (c_object.h)

```c
ObjectType_Undefined, ObjectType_Null, ObjectType_Array, ObjectType_Boolean,
ObjectType_Dictionary, ObjectType_Integer, ObjectType_Name, ObjectType_Real,
ObjectType_Stream, ObjectType_String, ObjectType_IndirectReference
```

### FieldType (c_fields.h)

```c
FieldType_Undefined, FieldType_NonTerminal, FieldType_Button,
FieldType_Text, FieldType_Choice, FieldType_Signature
```

### SignatureVerificationStatusType (c_signature_verifier.h)

```c
SignatureStatus_Undefined, SignatureStatus_Valid, SignatureStatus_Invalid,
SignatureStatus_CertificateExpired, SignatureStatus_CertificateNotYetValid,
SignatureStatus_CertificateRevoked, SignatureStatus_CertificateUntrusted,
SignatureStatus_DocumentModified, SignatureStatus_WeakAlgorithm,
SignatureStatus_MissingCertificate, SignatureStatus_Unknown
```

### MessageDigestAlgorithmType (c_message_digest_algorithm.h)

```c
MDNULL, MD2, MD4, MD5, SHA1, SHA224, SHA256, SHA384, SHA512, MDC2, RIPEMD160, WHIRLPOOL
```

## Function Signature Patterns

### Create Instance

```c
error_type Xxx_Create(XxxHandle** result);
error_type Xxx_CreateFromData(string_type data, size_type size, XxxHandle** result);
error_type Xxx_CreateFromFile(string_type filename, XxxHandle** result);
```

### Get/Set Properties

```c
error_type Xxx_GetProperty(XxxHandle* handle, PropertyType* result);
error_type Xxx_SetProperty(XxxHandle* handle, PropertyType value);
```

### Collection + Iterator

```c
error_type XxxCollection_GetSize(XxxCollectionHandle* handle, size_type* result);
error_type XxxCollection_At(XxxCollectionHandle* handle, size_type index, XxxHandle** result);

error_type Xxx_GetIterator(XxxHandle* handle, XxxIteratorHandle** result);
error_type XxxIterator_IsValid(XxxIteratorHandle* handle, boolean_type* result);
error_type XxxIterator_Next(XxxIteratorHandle* handle);
error_type XxxIterator_GetKey(XxxIteratorHandle* handle, KeyHandle** result);
error_type XxxIterator_GetValue(XxxIteratorHandle* handle, ValueHandle** result);
```

### Callback-Based Interfaces

```c
typedef error_type (*CallbackFunction)(void* user_data, ...);
error_type Xxx_CreateCustom(CallbackFunction cb1, CallbackFunction cb2, void* user_data, XxxHandle** result);
```

### Release

```c
error_type Xxx_Release(XxxHandle* handle);  // Wrapper around IUnknown_Release
```

## Implementation Bridge Layer (src/vanillapdf/implementation/)

### Casting Pattern

```cpp
// C handle → C++ object
Object* obj = reinterpret_cast<Object*>(handle);

// C++ object → C handle (with AddRef)
FilePtr file = File::Open(filename);
auto ptr = file.AddRefGet();  // Increments refcount
*result = reinterpret_cast<FileHandle*>(ptr);
```

### Helper Macros (c_helper.h)

```cpp
RETURN_ERROR_IF_NULL(var, error_code)         // Null check with specific error
RETURN_ERROR_PARAM_VALUE_IF_NULL(var)          // Null check with PARAMETER_VALUE error
CATCH_VANILLAPDF_EXCEPTIONS                    // Catches ExceptionBase → error code + stores message
```

### Template Helpers

```cpp
SafeObjectConvert<SourceT, DestT>(from, result)  // dynamic_cast + AddRef
ObjectRelease<T>(handle)                          // Cast + Release
```

### Error Translation Flow

1. C++ exception thrown in core code
2. `CATCH_VANILLAPDF_EXCEPTIONS` catches it
3. `e.code()` → cast to `error_type`
4. Message stored via `Errors::SetLastErrorMessage()`
5. Error code returned to C caller

## Export Macros (c_export.h)

```c
VANILLAPDF_API          // __declspec(dllexport/dllimport) on MSVC DLL, empty otherwise
CALLING_CONVENTION      // __cdecl on x86, empty on x64/ARM
VANILLAPDF_DEPRECATED   // __declspec(deprecated) or __attribute__((deprecated))
```

**Build definitions** (set in CMakeLists.txt):
- `VANILLAPDF_EXPORTS` - defined when building the library
- `VANILLAPDF_CONFIGURATION_DLL` - defined for shared lib consumers

**Deprecating a C API symbol**: when applying `VANILLAPDF_DEPRECATED` to a constant or function, update ALL internal references — `c_errors.cpp` error name lookup, `utils.c` integration test smoke tests, and any other usage. Clang `-Werror,-Wdeprecated-declarations` will fail the CI build if any reference is missed.

## Adding a New C API Function

1. Declare in appropriate `include/vanillapdf/*/c_*.h` header
2. Use pattern: `VANILLAPDF_API error_type CALLING_CONVENTION FunctionName(...);`
3. Implement wrapper in `src/vanillapdf/implementation/*/c_*.cpp`
4. Use `reinterpret_cast` for handle conversion
5. Use `RETURN_ERROR_PARAM_VALUE_IF_NULL` for parameter validation
6. Use `CATCH_VANILLAPDF_EXCEPTIONS` around C++ calls
7. Return handles via output parameter with `AddRefGet()`
8. Include header in `c_vanillapdf_api.h`

## Key API Functions Quick Reference

### File/Document Operations

```c
File_Open(filename, &file); File_Initialize(file);
Document_Open(filename, &doc); Document_Save(doc, filename);
Document_SaveIncremental(doc, filename);
Document_Sign(doc, dest_file, settings);
Document_AddEncryption(doc, settings); Document_RemoveEncryption(doc);
```

### Object Access

```c
Object_GetObjectType(handle, &type);
DictionaryObject_Find(dict, key, &result);
DictionaryObject_TryFind(dict, key, &found, &result);
ArrayObject_GetSize(arr, &size); ArrayObject_GetValue(arr, index, &result);
StreamObject_GetBody(stream, &buffer);  // Decompressed
StreamObject_GetBodyRaw(stream, &buffer);  // Compressed
```

### Signing & Verification

```c
PKCS12Key_CreateFromFile(path, password, &key);
PKCS12Key_ToSigningKey(key, &signing_key);
TrustedCertificateStore_Create(&store);
TrustedCertificateStore_LoadSystemDefaults(store);
SignatureVerifier_Verify(signed_data, signature, store, settings, &result);
SignatureVerificationResult_GetStatus(result, &status);
```
