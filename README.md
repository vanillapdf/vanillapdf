# Vanilla.PDF

[![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml)
[![Coverage](https://codecov.io/gh/vanillapdf/vanillapdf/branch/main/graph/badge.svg?token=1UO4W5XGTL)](https://codecov.io/gh/vanillapdf/vanillapdf)
[![Docs](https://readthedocs.org/projects/vanillapdf/badge/?version=latest)](https://vanillapdf.readthedocs.io/)
[![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue)](LICENSE.txt)

**Vanilla.PDF** is a C++17 library for creating, signing, encrypting, and parsing PDF documents. It exposes an ABI-stable ANSI C interface, compiles natively on Windows, Linux, macOS, and Android, and carries no external runtime dependencies.

The C API uses opaque handles and `cdecl` calling conventions, so any language that can call C functions can use this library. Error state is stored in thread-local buffers; objects are managed through intrusive reference counting. The public interface is designed to remain binary-compatible across minor and patch releases.

## Quick Example

Create a PDF with a blank page, then sign it:

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

```bash
vanillapdf-tools sign -s hello.pdf -d signed.pdf -k key.p12 -p password
vanillapdf-tools verify -f signed.pdf
```

## Non-Goals

Vanilla.PDF is a **document structure library**, not a rendering engine. It does not rasterize pages, lay out text with font shaping, or display PDFs on screen. If you need to view a PDF, use a dedicated renderer and this library for the structural operations around it.

Current known limitations:

- No PDF rendering or rasterization
- No CRL/OCSP revocation checking ([#157](https://github.com/vanillapdf/vanillapdf/issues/157))
- No PAdES compliance levels (BES, T, LTV)
- No RFC 3161 timestamp validation
- No ED25519 signature support ([#158](https://github.com/vanillapdf/vanillapdf/issues/158))

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

### Conan

```bash
conan install --requires="vanillapdf/2.3.0" --build=missing
```

### Homebrew (macOS)

```bash
brew install vanillapdf
```

### NuGet (.NET interop)

```bash
dotnet add package vanillapdf.net
```

### Build from Source

```bash
git clone https://github.com/vanillapdf/vanillapdf.git && cd vanillapdf
git submodule sync --recursive && git submodule update --init --recursive
cmake --preset linux-x64-gcc      # or windows-x64-msvc-17, macos-arm64
cmake --build --preset linux-x64-gcc
```

Then in your CMakeLists.txt:

```cmake
find_package(vanillapdf CONFIG REQUIRED)
target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

[Full installation guide](https://vanillapdf.readthedocs.io/en/latest/installation.html) | [Building from source](https://vanillapdf.readthedocs.io/en/latest/building.html)

## Features

| Feature | C API | CLI | Description |
|---------|:-----:|:---:|-------------|
| **Create documents** | `Document_Create` | | Pages, text, images, vector paths |
| **Digital signatures** | `Document_Sign` | `sign` | CMS (PKCS#7) with PKCS#12 keys or custom callbacks |
| **Signature verification** | `DigitalSignatureExtensions_Verify` | `verify` | Chain validation, weak-algorithm detection, signing-time checks |
| **Merge documents** | | `merge` | Combine multiple PDFs into one |
| **Encryption** | | `encrypt` / `decrypt` | AES and RC4, owner/user passwords, certificate-based decryption |
| **Image extraction** | | `extract` | JPEG and JPEG2000 from PDF streams |
| **Content streams** | `ContentStream_*` | `filter` | Parse and encode PostScript-style page content |
| **Low-level parsing** | `File_Open` | | XRef tables, indirect objects, cross-reference streams |

## Architecture

The library is organized into three layers:

- **Syntax** -- PDF object types, tokenizer, parser, XRef tables, compression filters
- **Semantics** -- Documents, pages, catalogs, annotations, digital signatures, forms
- **Contents** -- Content stream parsing, PostScript instruction processing

All internal C++ is hidden behind opaque C handles (`DocumentHandle*`, `FileHandle*`, `PageObjectHandle*`, etc.). Each handle is reference-counted; callers acquire and release references explicitly. This design guarantees ABI stability across compiler versions and enables bindings in any FFI-capable language.

## Platforms

| Platform | Compilers | Architectures |
|----------|-----------|---------------|
| Windows | Visual Studio 2022, 2026 | x86, x64 |
| Linux | GCC 8.1+, Clang 10+ | x64, ARM64, ARM |
| macOS | AppleClang 15+ (Xcode 15) | x64, ARM64 |
| Android | NDK toolchain | arm64, armv7, x86, x86_64 |

## Versioning

Vanilla.PDF follows [Semantic Versioning](https://semver.org/). The C API is stable within a major version: minor releases add functionality without breaking existing callers, patch releases contain only fixes. Query the version at runtime with `LibraryInfo_GetVersionMajor`, `LibraryInfo_GetVersionMinor`, `LibraryInfo_GetVersionPatch`.

## Documentation

Full documentation is hosted on **[Read the Docs](https://vanillapdf.readthedocs.io/)**.

| Guide | Description |
|-------|-------------|
| [Overview](https://vanillapdf.readthedocs.io/en/latest/overview.html) | Design philosophy, scope, and project goals |
| [Quickstart](https://vanillapdf.readthedocs.io/en/latest/quickstart.html) | Create your first PDF document step by step |
| [Installation](https://vanillapdf.readthedocs.io/en/latest/installation.html) | vcpkg, FetchContent, Conan, Homebrew, NuGet |
| [C API Guide](https://vanillapdf.readthedocs.io/en/latest/c_api.html) | Handles, memory management, error handling, thread safety |
| [Architecture](https://vanillapdf.readthedocs.io/en/latest/architecture.html) | Three-layer design, object model, memory model |
| [CLI Tools](https://vanillapdf.readthedocs.io/en/latest/cli_tools.html) | sign, verify, merge, extract, encrypt, decrypt |
| [Signature Verification](https://vanillapdf.readthedocs.io/en/latest/signature_verification.html) | Trust stores, chain validation, weak-algorithm detection |
| [Building](https://vanillapdf.readthedocs.io/en/latest/building.html) | Build from source with CMake presets |
| [Examples](https://vanillapdf.readthedocs.io/en/latest/examples.html) | Code samples for signing, merging, encryption |
| [PDF Format](https://vanillapdf.readthedocs.io/en/latest/pdf_format.html) | PDF syntax, objects, and document structure |

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
