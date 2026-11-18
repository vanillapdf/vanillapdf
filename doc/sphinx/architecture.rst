Architecture
============

This page describes the internal design of Vanilla.PDF: the layered
architecture, object model, memory model, thread-safety invariants, and
extension points.

For the public C API conventions (handles, error codes, cleanup patterns), see
:doc:`c_api`.

Three-layer architecture
------------------------

The library is organized into three layers, each with a distinct
responsibility. Higher layers depend on lower ones; the C interface layer
sits alongside all three and maps internal C++ objects to opaque handles.

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 18 30 52

   * - Layer
     - Source location
     - Responsibility
   * - **Syntax**
     - ``src/vanillapdf/syntax/``
     - PDF object types, tokenizer, parser, XRef tables, file I/O,
       compression filters (Flate, DCT, JPX, ASCII85, ASCIIHex, LZW)
   * - **Semantics**
     - ``src/vanillapdf/semantics/``
     - High-level document model: documents, catalogs, page trees, pages,
       annotations, interactive forms, digital signatures, outlines,
       named destinations, fonts, resource dictionaries
   * - **Contents**
     - ``src/vanillapdf/contents/``
     - Content stream parsing and instruction processing for PostScript-style
       page content (text operations, graphics state, inline images)

The C interface wrappers live in ``src/vanillapdf/implementation/`` and
translate between opaque handles and the internal C++ classes. Public headers
are in ``include/vanillapdf/``.

Why ANSI C?
^^^^^^^^^^^

The C++ ABI is not stable across compilers or across major versions of the
same compiler. A native C++ interface would force every consumer to use the
exact same toolchain. By exposing only ANSI C functions with ``cdecl`` calling
conventions, the library can be linked by any C or C++ compiler and called
from any language with a C FFI.

Functions across the interface use the standard C caller-cleanup
`cdecl <https://en.wikipedia.org/wiki/X86_calling_conventions#cdecl>`_
calling convention. On x86, this is enforced with ``__cdecl`` (MSVC) or
``__attribute__((cdecl))`` (GCC). On other architectures, the default calling
convention is used.

Object model
------------

Handle types
^^^^^^^^^^^^

All objects exposed through the C API are opaque handles — typed pointers to
internal C++ structures. Handles are declared using the
``DECLARE_OBJECT_HANDLE`` macro, which creates a forward-declared struct tag:

.. code-block:: c

   // From c_handles.h
   #define DECLARE_OBJECT_HANDLE(x) \
       typedef struct x##HandleTag x##Handle

This ensures that :cpp:class:`DocumentHandle* <DocumentHandle>` and :cpp:class:`FileHandle* <FileHandle>` are distinct pointer
types at the compiler level, preventing accidental misuse.

The handles fall into four categories matching the three layers plus utilities:

**Syntax layer handles** — PDF primitive objects and file structure:

- :cpp:class:`ObjectHandle`, :cpp:class:`ArrayObjectHandle`, :cpp:class:`BooleanObjectHandle`,
  :cpp:class:`NameObjectHandle`, :cpp:class:`IntegerObjectHandle`, :cpp:class:`RealObjectHandle`,
  :cpp:class:`StringObjectHandle`, :cpp:class:`StreamObjectHandle`, :cpp:class:`DictionaryObjectHandle`,
  :cpp:class:`NullObjectHandle`
- :cpp:class:`FileHandle`, :cpp:class:`FileWriterHandle`, :cpp:class:`XrefHandle`, :cpp:class:`XrefEntryHandle`
- Filter handles: :cpp:class:`FlateDecodeFilterHandle`, :cpp:class:`DCTDecodeFilterHandle`,
  :cpp:class:`ASCII85DecodeFilterHandle`, :cpp:class:`ASCIIHexDecodeFilterHandle`,
  :cpp:class:`LZWDecodeFilterHandle`, :cpp:class:`JPXDecodeFilterHandle`

**Semantics layer handles** — document-level objects:

- :cpp:class:`DocumentHandle`, :cpp:class:`CatalogHandle`, :cpp:class:`PageTreeHandle`,
  :cpp:class:`PageObjectHandle`, :cpp:class:`PageContentsHandle`
- :cpp:class:`DigitalSignatureHandle`, :cpp:class:`DocumentSignatureSettingsHandle`
- :cpp:class:`InteractiveFormHandle`, :cpp:class:`FieldHandle`, :cpp:class:`SignatureFieldHandle`
- :cpp:class:`OutlineHandle`, :cpp:class:`AnnotationHandle`, :cpp:class:`NamedDestinationsHandle`
- :cpp:class:`FontHandle`, :cpp:class:`ResourceDictionaryHandle`, :cpp:class:`DateHandle`,
  :cpp:class:`RectangleHandle`

**Contents layer handles** — content stream objects:

- :cpp:class:`ContentObjectHandle`, :cpp:class:`ContentOperationHandle`,
  :cpp:class:`ContentInstructionHandle`, :cpp:class:`ContentParserHandle`
- Text operations: :cpp:class:`ContentOperationTextFontHandle`,
  :cpp:class:`ContentOperationTextShowHandle`, :cpp:class:`ContentOperationBeginTextHandle`

**Utility handles** — I/O, buffers, cryptography:

- :cpp:class:`BufferHandle`, :cpp:class:`InputStreamHandle`, :cpp:class:`OutputStreamHandle`
- :cpp:class:`SigningKeyHandle`, :cpp:class:`EncryptionKeyHandle`, :cpp:class:`PKCS12KeyHandle`
- :cpp:class:`TrustedCertificateStoreHandle`,
  :cpp:class:`SignatureVerificationResultHandle`

Internal class hierarchy
^^^^^^^^^^^^^^^^^^^^^^^^

Internally, all syntax-layer objects inherit from ``Object``, which itself
derives from ``IUnknown`` (the reference-counting base):

.. code-block:: cpp

   // Simplified from src/vanillapdf/syntax/objects/object.h
   class Object : public virtual IUnknown,
                  public IWeakReferenceable<Object>,
                  public IModifyObservable {
       // Type system, offset tracking, XRef entry, dirty flag, attributes
   };

Semantics-layer objects use a template wrapper:

.. code-block:: cpp

   // From src/vanillapdf/semantics/objects/high_level_object.h
   template <typename T>
   class HighLevelObject : public virtual IUnknown {
       // Wraps a syntax-layer object with semantic meaning
   };

This separation means the semantic layer (:cpp:class:`DocumentHandle`,
:cpp:class:`PageObjectHandle`) operates on top of the syntax layer
(:cpp:class:`DictionaryObjectHandle`, :cpp:class:`StreamObjectHandle`) without exposing the
lower-level structure to callers.

Memory model
------------

Reference counting
^^^^^^^^^^^^^^^^^^

The library uses **intrusive reference counting**. The reference counter is
embedded directly inside each object, not in a separate control block. This
gives two advantages over ``std::shared_ptr``:

1. **Single allocation** — object and counter are allocated together
2. **Single dereference** — accessing the object does not require a second
   pointer hop through a control block

When an API function returns a handle through an output parameter, the caller
owns one reference. The caller must call the corresponding ``_Release``
function when done. Releasing a handle decrements the counter; when it reaches
zero, the object is destroyed.

.. code-block:: c

   DocumentHandle* doc = NULL;
   Document_Create("output.pdf", &doc);   // refcount = 1

   /* ... use the document ... */

   Document_Release(doc);                 // refcount = 0 → destroyed

**Rules:**

- Always initialize handles to ``NULL`` before use.
- Release handles in reverse order of acquisition.
- Never use a handle after releasing it.
- Double-releasing a handle is **undefined behavior**.

Weak references
^^^^^^^^^^^^^^^

The library supports weak references for objects that need to observe other
objects without preventing their destruction. Weak reference counters use
``std::atomic<bool>`` for thread-safe deactivation:

.. code-block:: cpp

   // From src/vanillapdf/utils/unknown_interface.h
   class WeakReferenceCounter {
       std::atomic<bool> m_active;
   };

.. _thread-safety:

Thread safety
-------------

Vanilla.PDF provides fine-grained thread safety: the **same object** can be
accessed concurrently from multiple threads — including objects that initialize
lazily on first access — without any caller-side locking. Internally, objects
are protected by mutexes and atomics.

**Error context is thread-local.** Each thread maintains its own error code
and message buffer:

.. code-block:: cpp

   // From src/vanillapdf/utils/errors.h
   static thread_local uint32_t m_error;
   static thread_local size_type m_message_length;
   static thread_local char m_message[constant::MAX_MESSAGE_SIZE];

The buffer is pre-allocated to avoid allocation failures when reporting
out-of-memory errors.

**Object-level locking.** Core PDF objects use ``std::recursive_mutex`` to
protect concurrent access. Each object owns its mutex via
``std::unique_ptr<std::recursive_mutex>``:

- ``DictionaryObject``
- ``StreamObject``
- ``StringObjectBase`` (literal and hexadecimal)
- ``IndirectReferenceObject``
- ``XrefUsedEntryBase``

**Reference counting is atomic.** ``IUnknown::m_ref_counter`` uses
``std::atomic<uint32_t>``, and ``WeakReferenceCounter::m_active`` uses
``std::atomic<bool>``, so reference management is safe across threads.

**Document opening is thread-safe.** ``Document::OpenFile`` atomically
returns the document already associated with a ``File`` instance, or creates
one, so threads that open the same ``File`` concurrently always receive the
same ``Document``. Each ``File::Open`` call creates a new ``File``, so opening
the same path twice yields two independent documents.

**Logging is thread-safe.** The logging subsystem uses ``spdlog``
multi-threaded sinks (``rotating_logger_mt``, ``custom_callback_sink_mt``
with internal ``std::mutex``). Writing log output from any thread is safe.

Error handling
--------------

Internally, the library uses C++ exceptions. Each C interface function wraps
its implementation in a try-catch block that catches all exceptions, stores
the message in the thread-local buffer, and returns an error code:

.. code-block:: c

   error_type Buffer_SetData(BufferHandle* handle, string_type data, size_type size) {
       Buffer* obj = reinterpret_cast<Buffer*>(handle);

       if (obj == nullptr) {
           return VANILLAPDF_ERROR_PARAMETER_VALUE;
       }

       try {
           obj->assign(data, data + size);
           return VANILLAPDF_ERROR_SUCCESS;
       } catch (std::exception& e) {
           // Store e.what() in thread-local buffer
           return VANILLAPDF_ERROR_GENERAL;
       } catch (...) {
           return VANILLAPDF_ERROR_GENERAL;
       }
   }

.. note::

   The wrapping try-catch has
   `negligible performance impact <https://en.wikipedia.org/wiki/Exception_handling#Exception_handling_implementation>`_
   on modern compilers using table-based exception handling.

Extension points
----------------

The library provides callback interfaces that callers can implement to
customize behavior without modifying the library itself.

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 35 65

   * - Interface
     - Purpose
   * - :cpp:class:`SigningKeyHandle`
     - Custom signing implementation for smart cards or HSMs where the
       private key is not directly accessible. Override instead of using
       :cpp:class:`PKCS12KeyHandle` when keys cannot be exported.
   * - :cpp:class:`EncryptionKeyHandle`
     - Custom encryption key provider for certificate-based decryption
       or external key management systems.
   * - :cpp:class:`FileWriterObserverHandle`
     - Callback interface invoked during PDF file writing. Allows callers
       to monitor or modify the output process.

For signing, the standard approach uses PKCS#12 key files
(`RFC 7292 <https://www.rfc-editor.org/rfc/rfc7292>`_):

.. code-block:: c

   PKCS12KeyHandle* pkcs12 = NULL;
   SigningKeyHandle* key = NULL;
   DocumentSignatureSettingsHandle* settings = NULL;

   PKCS12Key_CreateFromFile("key.p12", "password", &pkcs12);
   PKCS12Key_ToSigningKey(pkcs12, &key);

   DocumentSignatureSettings_Create(&settings);
   DocumentSignatureSettings_SetSigningKey(settings, key);
   Document_Sign(doc, destination, settings);

When the private key resides on a smart card or HSM, implement
:cpp:class:`SigningKeyHandle` directly and provide your own signing logic.
See ``sign_custom.c`` for an example.

File layer internals
--------------------

The file layer provides access to PDF file contents at the syntactic level.

IO streams
^^^^^^^^^^

The library uses C++ I/O streams internally. The public interface exposes
two stream types:

- :cpp:class:`InputStreamHandle` — for reading source files
- :cpp:class:`OutputStreamHandle` — for writing output files

How a file is read and written is selected with :cpp:enum:`IOStrategyType`, passed to
:cpp:func:`File_OpenWithStrategy <FileHandle::File_OpenWithStrategy>`, :cpp:func:`File_CreateWithStrategy <FileHandle::File_CreateWithStrategy>`,
:cpp:func:`Document_OpenWithStrategy <DocumentHandle::Document_OpenWithStrategy>` and :cpp:func:`Document_CreateWithStrategy <DocumentHandle::Document_CreateWithStrategy>`. The default
uses C++ ``fstream``; ``IOStrategy_FileStream`` uses ``FILE*`` based streaming.
The in-memory and memory-mapped strategies are reserved and not yet supported.
Custom I/O backends cannot be plugged in.

Tokenizer
^^^^^^^^^

The tokenizer produces the smallest syntactic elements, separated by
whitespace or delimiters as defined in
`section 7.2 of the PDF specification <_static/PDF32000_2008.pdf#G6.1638740>`_.

The tokenizer uses look-ahead to disambiguate tokens. For example,
``<`` begins a hexadecimal string, while ``<<`` begins a dictionary:

.. code-block:: c

   int c = m_stream->Get();
   if (c == Delimiter::LESS_THAN_SIGN) {
       int ahead = m_stream->Peek();
       if (ahead == Delimiter::LESS_THAN_SIGN) {
           return Token::Type::DICTIONARY_BEGIN;
       }
       // ... read hex string ...
       return Token::Type::HEXADECIMAL_STRING;
   }

Parser
^^^^^^

The parser consumes tokens and constructs PDF objects. It also uses
look-ahead, since multiple tokens may form a single object (e.g., an
indirect reference is three tokens: object number, generation number, ``R``).

.. image:: /_images/indirect_reference_parsing.png
   :alt: Diagram for parsing indirect object references

Indirect references
^^^^^^^^^^^^^^^^^^^

:cpp:class:`IndirectReferenceObjectHandle` must often be resolved to access the
referenced object. For example, :cpp:class:`StreamObjectHandle` frequently stores its
``Length`` as an indirect reference. The file layer resolves these references
transparently during parsing.

Quality assurance
-----------------

Beyond the unit and integration test suites, the project runs two additional
correctness lines:

- **Fuzzing.** libFuzzer-based targets under ``src/vanillapdf.fuzzer/`` exercise
  the parser, content streams, and decompression filters (Flate, DCT, JPX,
  ASCII85, ASCIIHex) against a seed corpus. Continuously run via
  ``.github/workflows/fuzzing.yml``.
- **External conformance.** ``scripts/conformance_check.py`` re-saves selected
  files of the test corpus and checks the output with
  `qpdf <https://qpdf.readthedocs.io/>`_ (``qpdf --check``) to catch malformed
  files that pass the library's own checks. Continuously run via
  ``.github/workflows/conformance-check.yml``.

Both feed back into the test corpus when they find issues.

Dependencies
------------

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 25 75

   * - Library
     - Purpose
   * - `OpenSSL <https://www.openssl.org/>`_
     - Encryption, decryption, and digital signatures
   * - `zlib <https://zlib.net/>`_
     - Flate compression
   * - `libjpeg-turbo <https://libjpeg-turbo.org/>`_
     - JPEG image decoding
   * - `OpenJPEG <https://www.openjpeg.org/>`_
     - JPEG2000 image support
   * - `spdlog <https://github.com/gabime/spdlog>`_
     - Logging
   * - `fmt <https://fmt.dev/>`_
     - String formatting
   * - `nlohmann-json <https://github.com/nlohmann/json>`_
     - License metadata (optional)
   * - `mimalloc <https://github.com/microsoft/mimalloc>`_
     - Allocator of the reference counted objects (optional)

nlohmann-json and mimalloc are only used when ``VANILLAPDF_ENABLE_LICENSING`` or
``VANILLAPDF_ENABLE_MIMALLOC`` is on; both are off by default.

All dependencies are managed automatically via vcpkg. See :doc:`building` for
details on using system packages instead.

Build system
------------

The build uses CMake with platform-specific presets defined in
``cmake/presets/``. CPack generates the installable packages:

- Debian ``.deb`` and RPM ``.rpm`` packages on Linux
- ``.tar.gz`` archives on Linux and macOS, and a ``.dmg`` disk image on macOS
- ``.zip`` archives on Windows

NuGet packages are built with ``dotnet pack`` from ``nuget/``, and the
Homebrew formula is generated from ``homebrew/vanillapdf.rb.in``.

See :doc:`building` for build instructions and :doc:`packaging` for package
generation.
