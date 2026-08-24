# Vanilla.PDF – The Ultimate C++ PDF Toolkit

<div align="center">

| Distribution | CI & Quality | Security |
|:---:|:---:|:---:|
| [![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf) [![Downloads](https://img.shields.io/nuget/dt/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf) [![Docs](https://readthedocs.org/projects/vanillapdf/badge/?version=latest)](https://vanillapdf.readthedocs.io/) [![License](https://img.shields.io/badge/license-Apache%202.0-blue)](LICENSE.txt) | [![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml) [![Coverage](https://codecov.io/gh/vanillapdf/vanillapdf/branch/main/graph/badge.svg?token=1UO4W5XGTL)](https://codecov.io/gh/vanillapdf/vanillapdf) [![Fuzzing](https://github.com/vanillapdf/vanillapdf/actions/workflows/fuzzing.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/fuzzing.yml) [![CodeQL](https://github.com/vanillapdf/vanillapdf/actions/workflows/codeql.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/codeql.yml) [![Sanitizers](https://github.com/vanillapdf/vanillapdf/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/sanitizers.yml) | [![OpenSSF Scorecard](https://api.scorecard.dev/projects/github.com/vanillapdf/vanillapdf/badge)](https://scorecard.dev/viewer/?uri=github.com/vanillapdf/vanillapdf) [![SLSA 2](https://slsa.dev/images/gh-badge-level2.svg)](https://slsa.dev) |

</div>

**Vanilla.PDF** is a modern, high-performance, open-source C++17 SDK for creating, editing, signing, and analyzing PDF documents. With no external runtime dependencies and full cross-platform support, it's ideal for embedding into desktop, server, or automation workflows.

- 📄 **Create & modify** PDF documents — pages, text, images, vector graphics
- 🔏 **Sign & verify** digital signatures (CMS/PKCS#7) with certificate chain validation
- 🔒 **Encrypt & decrypt** with AES or RC4 using passwords or certificates
- ⚙️ **ABI-stable C API** — opaque handles callable from any language with a C FFI
- 🧵 **Thread-safe** — no global state; synchronization is handled internally

## Install

### vcpkg (Recommended)

```bash
vcpkg install vanillapdf
```

### CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(vanillapdf
    GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
    GIT_TAG        main)
FetchContent_MakeAvailable(vanillapdf)
target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

### Other methods

- **NuGet** (.NET interop) — `dotnet add package vanillapdf.net`
- **Conan** 🚧 — coming soon (not yet published to Conan Center)
- **Homebrew** (macOS) 🚧 — coming soon (formula not yet live in Homebrew core)
- **Build from source** — clone with submodules, then configure with a CMake preset

See the [installation guide](https://vanillapdf.readthedocs.io/en/latest/installation.html) and [building from source](https://vanillapdf.readthedocs.io/en/latest/building.html) on Read the Docs for full instructions.

## Quick Example

Create a PDF with a blank page:

```c
#include <vanillapdf/c_vanillapdf_api.h>

int main(void) {
    DocumentHandle* doc = NULL;
    CatalogHandle* cat = NULL;
    PageTreeHandle* pages = NULL;
    PageObjectHandle* page = NULL;

    Document_Create("hello.pdf", &doc);
    Document_GetCatalog(doc, &cat);
    Catalog_GetPages(cat, &pages);
    PageObject_CreateFromDocument(doc, &page);
    PageTree_AppendPage(pages, page);
    Document_Save(doc, "hello.pdf");

    PageObject_Release(page);
    PageTree_Release(pages);
    Catalog_Release(cat);
    Document_Release(doc);
    return 0;
}
```

## Platforms

| Platform | Compilers | Architectures |
|----------|-----------|---------------|
| Windows | Visual Studio 2022, 2026 | x86, x64, ARM64 |
| Linux | GCC 8.1+, Clang 10+ | x64, ARM64, ARM |
| macOS | AppleClang 15+ (Xcode 15) | x64, ARM64 |
| Android | NDK toolchain | arm64, armv7, x86, x86_64 |

## Features

| Feature | C API | Description |
|---------|-------|-------------|
| **Create documents** | `Document_Create` | Pages, text, images, vector paths |
| **Digital signatures** | `Document_Sign` | OpenSSL CMS — RSA, ECDSA (P-256/P-384/P-521), Ed25519, Ed448 via PKCS#12 keys or custom callbacks |
| **Signature verification** | `DigitalSignatureExtensions_Verify` | Chain validation, weak-algorithm detection, signing-time checks |
| **File structure validation** | `FileStructureValidator_*` | Walk xref/trailers/streams and report malformed files |
| **Interactive forms** | `FormField_GetValue` / `_SetValue` | Read/write AcroForm field values |
| **Encryption** | `Document_AddEncryption` / `_RemoveEncryption` | AES and RC4, owner/user passwords, certificate-based decryption (FIPS-compatible) |
| **Content streams** | `ContentStream_*` | Parse and encode PostScript-style page content |
| **Low-level parsing** | `File_Open` | XRef tables, indirect objects, cross-reference streams |

## Architecture

The library is organized into three layers:

- **Syntax** — PDF object types, tokenizer, parser, XRef tables, compression filters
- **Semantics** — Documents, pages, catalogs, annotations, digital signatures, forms
- **Contents** — Content stream parsing, PostScript instruction processing

The C++ core is hidden behind an ABI-stable ANSI C interface using opaque handles (`DocumentHandle*`, `FileHandle*`, `PageObjectHandle*`, etc.) and `cdecl` calling conventions, so any language with a C FFI can use this library. Handles are reference-counted; callers acquire and release references explicitly. This design guarantees binary compatibility across compiler versions and minor/patch releases.

## Thread Safety

Vanilla.PDF provides fine-grained thread safety: the **same object** can be accessed concurrently from multiple threads — including objects that initialize lazily on first access — without any caller-side locking. This is a stronger guarantee than merely running separate documents on separate threads; synchronization *within* a shared object is handled internally.

See the [Architecture Guide](https://vanillapdf.readthedocs.io/en/latest/architecture.html) for the memory and locking model.

## Non-Goals

Vanilla.PDF is a **document structure library**, not a rendering engine. It does not rasterize pages, lay out text with font shaping, or display PDFs on screen. If you need to view a PDF, use a dedicated renderer and this library for the structural operations around it.

Current known limitations:

- No PDF rendering or rasterization
- No CRL/OCSP revocation checking ([#157](https://github.com/vanillapdf/vanillapdf/issues/157))
- No PAdES compliance levels (BES, T, LTV)
- No RFC 3161 timestamp validation

## Versioning

Vanilla.PDF follows [Semantic Versioning](https://semver.org/). The C API is stable within a major version: minor releases add functionality without breaking existing callers, patch releases contain only fixes. Query the version at runtime with `LibraryInfo_GetVersionMajor`, `LibraryInfo_GetVersionMinor`, `LibraryInfo_GetVersionPatch`.

## Documentation

Full documentation is hosted on **[Read the Docs](https://vanillapdf.readthedocs.io/)**.

| Guide | Description |
|-------|-------------|
| [Overview](https://vanillapdf.readthedocs.io/en/latest/overview.html) | Design philosophy, scope, and project goals |
| [Quickstart](https://vanillapdf.readthedocs.io/en/latest/quickstart.html) | Create your first PDF document step by step |
| [Installation](https://vanillapdf.readthedocs.io/en/latest/installation.html) | vcpkg, FetchContent, Conan, Homebrew, NuGet |
| [C API Guide](https://vanillapdf.readthedocs.io/en/latest/c_api.html) | Handles, memory management, error handling |
| [Architecture](https://vanillapdf.readthedocs.io/en/latest/architecture.html) | Three-layer design, object model, memory model, thread safety |
| [Signature Verification](https://vanillapdf.readthedocs.io/en/latest/signature_verification.html) | Trust stores, chain validation, weak-algorithm detection |
| [Building](https://vanillapdf.readthedocs.io/en/latest/building.html) | Build from source with CMake presets |
| [Examples](https://vanillapdf.readthedocs.io/en/latest/examples.html) | Code samples for signing, merging, encryption |
| [PDF Format](https://vanillapdf.readthedocs.io/en/latest/pdf_format.html) | PDF syntax, objects, and document structure |

See the [Changelog](CHANGELOG.md) for release notes.

## Contributing

We welcome pull requests, feature proposals, and bug reports.

- [Contributing Guidelines](CONTRIBUTING.md)
- [Code of Conduct](CODE_OF_CONDUCT.md)

All changes require a branch and pull request. The `main` and `release/*` branches are protected.

## License

Vanilla.PDF is licensed under the [Apache 2.0 License](LICENSE.txt).

## Contact & Support

| Channel | Link |
|---------|------|
| Email | info@vanillapdf.com |
| Website | [vanillapdf.com](https://vanillapdf.com) |
| Issues | [GitHub Issues](https://github.com/vanillapdf/vanillapdf/issues) |
| Sponsor | [GitHub Sponsors](https://github.com/sponsors/vanillapdf) |
