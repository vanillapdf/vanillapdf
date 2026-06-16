# Architecture

## Core Structure

- **src/vanillapdf/** - Main library (C++17): `syntax/`, `semantics/`, `contents/`, `utils/`, `implementation/` (C bridge)
- **src/vanillapdf.tools/** - CLI utility (C)
- **src/vanillapdf.test/** - Integration tests (C, one test per PDF)
- **src/vanillapdf.unittest/** - Unit tests (GoogleTest, C++)
- **src/vanillapdf.benchmark/** - Benchmarks (Google Benchmark, C++)
- **src/vanillapdf.fuzzer/** - Fuzz targets (libFuzzer, C++), gated by `VANILLAPDF_ENABLE_FUZZING` (requires Clang)

## Layered Architecture

1. **Syntax Layer** (`syntax/`) - PDF objects, parsing, file structure
2. **Semantics Layer** (`semantics/`) - Pages, annotations, forms, signatures
3. **Contents Layer** (`contents/`) - Content stream parsing and operations

## Object Model

### Type Hierarchy (syntax/objects/)

`Object` (abstract, Versionable) → `ContainableObject` (abstract) with concrete types:
- **Collections**: `MixedArrayObject`, `ArrayObject<T>`, `DictionaryObject` (thread-safe)
- **Strings**: `LiteralStringObject`, `HexadecimalStringObject` (thread-safe)
- **Numerics**: `IntegerObject`, `RealObject`
- **Simple**: `BooleanObject`, `NameObject`, `NullObject` (singleton)
- **References**: `IndirectReferenceObject` (lazy-resolving, thread-safe)
- **Streams**: `StreamObject` (dict header + raw data, thread-safe, lazy decode)

Type enum: `Object::Type { Null, Array, Boolean, Dictionary, Integer, Name, Real, Stream, String, IndirectReference }`

### Smart Pointers

- **`Deferred<T>`** (`utils/deferred.h`): Ref-counted pointer via `IUnknown::AddRef()/Release()`. Create with `make_deferred<T>(...)`. Common typedefs: `ObjectPtr`, `StringObjectPtr`, `DictionaryObjectPtr`, `ArrayObjectPtr<T>`
- **`WeakReference<T>`** (`utils/unknown_interface.h`): Prevents use-after-free via `std::atomic<bool>` counter. Check `IsActive()`, promote with `GetReference()`
- **`IUnknown`**: Base class with `std::atomic<uint32_t>` ref counter; `Release()` deletes at 0

### Object Lifecycle

Creation → `SetFile()` → `SetXrefEntry()` → `SetOwner()` → mutations call `IncrementVersion()` → `ToPdfStream()` → `Release()`

## Parser Architecture (syntax/parsers/)

`Tokenizer` → `ParserBase` → `Parser` (full PDF: xrefs, indirect objects, streams)
`ReverseTokenizer` → `ReverseParser` (finds xref offset from EOF)

## Content Stream Processing (contents/)

`InstructionBase` → `PDFObject` (wraps syntax Object) or `OperationBase` (88 operation types)
`ContentStreamParser` produces `BaseInstructionCollection` from stream content.

## Semantic Layer (semantics/objects/)

All inherit `HighLevelObject<T>` wrapping a syntax object (typically `DictionaryObjectPtr`).

Key classes: `Document` (Open/Create/Save/Sign), `Catalog`, `PageObject`/`PageTree`, `InteractiveForm`/`FormField`, `DigitalSignature`, `Annotations`, `Destinations`, `NameDictionary`/`NameTree`, `CharacterMap`

## Exception Hierarchy (utils/exceptions.h)

`ExceptionBase : std::exception` with `Type` enum mapping 1:1 to C API error codes via `CATCH_VANILLAPDF_EXCEPTIONS`. Key types: `InvalidParameter`, `NotSupported`, `ParseException`, `ObjectMissing`, `InvalidPassword`, `OptionalEntryMissing`, `General`

**When to use each type:**
- `NotSupportedException` — valid input, functionality not implemented (e.g., "compiled without OpenSSL", "TIFF predictor not supported"). Not for unknown/unexpected values.
- `ParseException` — unknown or malformed values read from PDF files
- `InvalidParameterException` — invalid user-supplied values or violated internal invariants
- Base exception classes (`InvalidParameterException`, etc.) are available everywhere via `precompiled.h` — no extra include needed
- In `semantics/`, `ParseException` lives in the `syntax::` namespace: add `#include "syntax/exceptions/syntax_exceptions.h"` and use `syntax::ParseException` (or `using namespace syntax;`)

## Dirty Tracking

Poll-based `Versionable` with `std::atomic<uint32_t> m_version`. Leaf: dirty if version > 0. Containers: check own version + iterate children.

## Thread Safety

The library is thread-safe. Key objects use `std::recursive_mutex` for concurrent access, and reference counting is atomic:
- `IUnknown::m_ref_counter` - `std::atomic<uint32_t>`
- `WeakReferenceCounter::m_active` - `std::atomic<bool>`
- `DictionaryObject`, `StreamObject`, `StringObjectBase`, `IndirectReferenceObject`, `XrefUsedEntryBase` - `std::unique_ptr<std::recursive_mutex> _access_lock`
- `Document::OpenFile` - atomically returns existing or creates new document per file
- Streams: `ExclusiveInputLock()` / `ExclusiveInputUnlock()`
- Error context: `thread_local` buffers (no cross-thread interference)
- Logging: `spdlog` multi-threaded sinks

Thread safety is validated by `thread_safety_test.cpp` (concurrent `Document::OpenFile` with 50 threads x 2000 iterations).

## Feature Dependencies

Optional via `VANILLAPDF_HAVE_*`: OpenSSL (encryption/signing), libjpeg-turbo, openjpeg, zlib, spdlog, nlohmann-json
