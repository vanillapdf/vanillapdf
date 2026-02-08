# Architecture

## Core Structure

- **src/vanillapdf/** - Main library implementation (C++17)
  - **syntax/** - Low-level PDF syntax parsing and objects
  - **semantics/** - High-level PDF document semantics
  - **contents/** - Content stream parsing and operations
  - **utils/** - Utility classes and interfaces
  - **implementation/** - C interface bridge layer
- **src/vanillapdf.tools/** - CLI utility (C code)
- **src/vanillapdf.test/** - Integration tests (C code, one test per PDF)
- **src/vanillapdf.unittest/** - Unit tests (GoogleTest, C++)
- **src/vanillapdf.benchmark/** - Benchmarks (Google Benchmark, C++)

## Layered Architecture

Three distinct layers, each with C++ internals and a C API surface:

1. **Syntax Layer** - Low-level PDF objects and file structure
2. **Semantics Layer** - High-level document concepts (pages, annotations, forms)
3. **Contents Layer** - Content stream parsing and operations

## Object Model - Class Hierarchy

### PDF Object Hierarchy (syntax/objects/)

```
Object (abstract) : IUnknown, IWeakReferenceable<Object>, IModifyObservable
├── ContainableObject (abstract marker - objects that go inside arrays/dicts)
│   ├── MixedArrayObject (concrete, stores vector<ContainableObjectPtr>)
│   │   └── ArrayObject<T> (template wrapper with type-safe access)
│   ├── DictionaryObject (concrete, stores map<NameObjectPtr, ContainableObjectPtr>)
│   │   └── Thread-safe: uses std::shared_ptr<std::recursive_mutex>
│   ├── StringObjectBase (abstract)
│   │   ├── LiteralStringObject (concrete, parenthesis-delimited)
│   │   └── HexadecimalStringObject (concrete, angle-bracket-delimited)
│   │   └── Both thread-safe with std::shared_ptr<std::recursive_mutex>
│   ├── NumericObject (abstract, backed by NumericObjectBackend)
│   │   ├── IntegerObject (concrete)
│   │   └── RealObject (concrete)
│   ├── BooleanObject (concrete)
│   ├── NameObject (concrete)
│   ├── NullObject (singleton, concrete)
│   └── IndirectReferenceObject (concrete, lazy-resolving weak reference)
│       └── Thread-safe with std::recursive_mutex
└── StreamObject (concrete, DictionaryObject header + raw data)
    └── Thread-safe with std::shared_ptr<std::recursive_mutex>
    └── Lazy decoding: _body_raw → _body_decrypted → _body_decoded
```

### Object Type Enum

```cpp
Object::Type { Null, Array, Boolean, Dictionary, Integer, Name, Real, Stream, String, IndirectReference }
```

### Key Object Methods

- `GetObjectType()` - Returns Type enum
- `ToPdf()`, `ToPdfStream()` - Serialize to PDF format
- `IsIndirect()`, `GetXrefEntry()` - Cross-reference association
- `IsDirty()`, `SetDirty()` - Change tracking
- `GetOffset()`, `SetOffset()` - File position (bytes from start)
- `Hash()`, `Equals()` - Equality and hashing
- `Clone()` - Deep copy
- `SetFile()`, `GetFile()` - File association (weak reference)
- `SetOwner()`, `GetOwner()` - Parent-child ownership

### Object Lifecycle

1. **Creation**: Via constructors, parser, or `make_deferred<T>(...)`
2. **File Association**: `SetFile(WeakReference<File>)` links to owning PDF
3. **Xref Registration**: `SetXrefEntry()` registers in cross-reference table
4. **Ownership**: `SetOwner()` establishes parent-child relationships
5. **Mutation**: Changes trigger `OnChanged()` → observer notifications
6. **Serialization**: `ToPdfStream()` outputs to PDF format
7. **Cleanup**: `Release()` via reference counting

## Smart Pointer System

### Deferred<T> (utils/deferred.h) - Primary Smart Pointer

Reference-counted pointer that integrates with `IUnknown::AddRef()/Release()`:

```cpp
// Creation
ObjectPtr obj = make_deferred<IntegerObject>(42);

// Copy (increments ref count)
ObjectPtr copy = obj;

// Move (steals ownership, zeros source)
ObjectPtr moved = std::move(obj);

// Dereference
int value = (*obj)->GetValue();

// Raw pointer with incremented refcount (for C API returns)
Object* ptr = obj.AddRefGet();

// Automatic weak reference conversion
WeakReference<Object> weak = obj;
```

**Common Typedefs**:
- `ObjectPtr` = `Deferred<Object>`
- `StringObjectPtr` = `Deferred<StringObjectBase>`
- `DictionaryObjectPtr` = `Deferred<DictionaryObject>`
- `ArrayObjectPtr<T>` = `Deferred<ArrayObject<T>>`

### WeakReference<T> (utils/unknown_interface.h)

Prevents use-after-free via shared counter:

```cpp
WeakReference<Object> weak = obj.GetWeakReference();
if (weak.IsActive()) {
    ObjectPtr strong = weak.GetReference(); // Throws if disposed
}
```

Storage: `{T* m_ptr, shared_ptr<WeakReferenceCounter> m_counter}` where counter uses `std::atomic<bool>`.

## IUnknown Reference Counting

All objects inherit from `IUnknown`:

```cpp
class IUnknown {
    std::atomic<uint32_t> m_ref_counter;
public:
    void AddRef();    // Atomic increment
    void Release();   // Atomic decrement; delete when 0
};
```

## Parser Architecture (syntax/parsers/)

```
Tokenizer (base - character-by-character token reading with offset cache)
├── ParserBase (extends Tokenizer - converts tokens to Object instances)
│   └── Parser (full PDF parsing - xrefs, indirect objects, streams)
└── ReverseTokenizer (reads backwards from end of file)
    └── ReverseParser (finds xref offset from EOF)
```

### Tokenizer

- Reads tokens from input stream character-by-character
- Caching: `std::map<stream_offset, CacheItem>` avoids re-reads
- Key methods: `ReadToken()`, `PeekToken()`, `ReadTokenWithType()`
- Private readers: `ReadComment()`, `ReadHexadecimalString()`, `ReadLiteralString()`, `ReadName()`, `ReadUnknown()`

### ParserBase

- Stores `WeakReference<File>` for object context
- `ReadDirectObject()` dispatches to type-specific readers
- `ReadArray()`, `ReadDictionary()`, `ReadDictionaryStream()`
- Templated: `ReadDirectObjectWithType<T>()`

### Parser (implements IParser)

- `ReadXref()` - Reads from offset or current position
- `ReadIndirectObject()` - Reads `obj gen obj ... endobj`
- `ReadObjectStreamEntries()` - Parses object streams (PDF 1.5+)
- `ReadHeader()` - Extracts `%PDF-X.Y` version
- `FindAllObjects()` - Builds complete xref chain
- Private: `ReadXrefTable()`, `ReadXrefStream()`, `ParseXrefStream()`

### ReverseParser

- Finds xref offset from end of file (reverse scanning)
- Key method: `ReadLastXrefOffset()`
- Used in `File::Initialize()` to locate trailer

## Content Stream Processing (contents/)

### Instruction Hierarchy

```
InstructionBase (abstract - token or operation)
├── PDFObject (wraps syntax::Object)
└── OperationBase (abstract - PDF operation, 88 types)
    ├── GenericOperation (parameterized)
    └── 88 specific operations (LineWidth, SaveGraphicsState, TextFont, TextShow, etc.)

BaseInstructionCollection (container of InstructionBase)
└── STL-compatible: begin(), end(), at(), push_back()
```

### Operation Types (OperationBase::Type enum, 88 values)

Categories: Graphics state (5), Path construction (10), Path painting (8), Text operations (16), Color operations (8), Inline images, Marked content, XObject, Shading, etc.

### ContentStreamParser

- Input: Stream content (compressed/decompressed)
- Output: `BaseInstructionCollection` of mixed objects and operations
- Handles CID→Unicode mapping for text via character maps
- Handles inline images (BI...ID...EI structure)

## Semantic Layer (semantics/objects/)

### Base Class: HighLevelObject<T>

```cpp
template <typename T>
class HighLevelObject : public virtual IUnknown {
protected:
    T _obj;  // Wrapped syntax object (typically DictionaryObjectPtr)
};
```

### Key Semantic Classes

- **Document** - Top-level. Static factories: `Open()`, `Create()`. Methods: `Save()`, `SaveIncremental()`, `Sign()`, `AddEncryption()`, `RemoveEncryption()`, `AppendDocument()`
- **Catalog** - Root of document structure. Contains Pages, Names, AcroForm
- **PageObject** / **PageTree** - Hierarchical page structure
- **InteractiveForm** / **FormField** - AcroForm objects and field definitions
- **DigitalSignature** - Signature dictionary with contact info, reason, certificate
- **Annotations** - Page annotations (links, comments, etc.)
- **Destinations** - Named destinations for links/jumps
- **NameDictionary** / **NameTree** - Hierarchical name storage
- **CharacterMap** - CID font character mappings

### Extension Pattern

```cpp
class DigitalSignatureExtensions {
public:
    static SignatureVerificationResultPtr Verify(
        DigitalSignaturePtr signature, DocumentPtr document,
        TrustedCertificateStorePtr trusted_store,
        SignatureVerificationSettingsPtr settings
    );
};
```

## Exception Hierarchy (utils/exceptions.h)

```cpp
ExceptionBase : std::exception
├── Type enum (uint32_t):
│   Success = 0, InvalidParameter, NotSupported, UserCancelled,
│   ZlibDataError, InvalidLicense, LicenseRequired, InsufficientSpace,
│   Conversion = 0x00010000, FileDisposed, FileNotInitialized,
│   ObjectMissing, ParseException, InvalidPassword, DuplicateKey,
│   OptionalEntryMissing = 0x10000000, SemanticContext,
│   General = 0xFFFFFFFF
```

Maps 1:1 to C API error codes via `CATCH_VANILLAPDF_EXCEPTIONS` macro.

## Observer Pattern

- `IModifyObservable` / `IModifyObserver` for change notifications
- Objects notify observers on mutation via `OnChanged()`
- Used for dirty tracking and cascading updates through object graph

## Thread Safety

The library is thread-safe. Key objects use `std::recursive_mutex` for concurrent access, and reference counting is atomic:
- `IUnknown::m_ref_counter` - `std::atomic<uint32_t>`
- `WeakReferenceCounter::m_active` - `std::atomic<bool>`
- `DictionaryObject` - `std::shared_ptr<std::recursive_mutex> m_access_lock`
- `StreamObject` - `std::shared_ptr<std::recursive_mutex> _access_lock`
- `StringObjectBase` (both variants) - `std::shared_ptr<std::recursive_mutex> _access_lock`
- `IndirectReferenceObject` - `std::recursive_mutex m_access_lock`
- `XrefUsedEntryBase` - `std::shared_ptr<std::recursive_mutex> m_access_lock`
- `Document::OpenFile` - atomically returns existing or creates new document per file
- Streams: `ExclusiveInputLock()` / `ExclusiveInputUnlock()`
- Error context: `thread_local` buffers (no cross-thread interference)
- Logging: `spdlog` multi-threaded sinks

Thread safety is validated by `thread_safety_test.cpp` (concurrent `Document::OpenFile` with 50 threads x 2000 iterations).

## Feature Dependencies

Optional, controlled by `VANILLAPDF_HAVE_*` compile definitions:
- OpenSSL (`VANILLAPDF_HAVE_OPENSSL`): Encryption/decryption and digital signing
- libjpeg-turbo (`VANILLAPDF_HAVE_JPEG`): JPEG image support
- openjpeg (`VANILLAPDF_HAVE_OPENJPEG`): JPEG 2000 support
- zlib (`VANILLAPDF_HAVE_ZLIB`): PDF object compression
- spdlog: Logging framework
- nlohmann-json: Configuration parsing

## Development Tools

- `.natvis` files: Visual Studio debugger visualizations (`public.natvis`, `vanillapdf.natvis`)
- `precompiled.h` includes: `deferred.h`, `constants.h`, `exceptions.h`, `log.h`, `util.h`, `object_utils.h`, `objects.h`
- Debug memory tracking on MSVC: `_CRTDBG_MAP_ALLOC` with `pdf_new` macro
