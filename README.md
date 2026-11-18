# Vanilla.PDF

**The C and C++ library for creating, editing, signing and validating PDF documents.**

<div align="center">

| Distribution | CI & Quality | Security |
|:---:|:---:|:---:|
| [![vcpkg](https://img.shields.io/vcpkg/v/vanillapdf?color=blue)](https://vcpkg.io/en/package/vanillapdf) [![Docs](https://readthedocs.org/projects/vanillapdf/badge/?version=latest)](https://vanillapdf.readthedocs.io/) [![License](https://img.shields.io/badge/license-Apache%202.0-blue)](LICENSE.txt) | [![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml) [![Coverage](https://codecov.io/gh/vanillapdf/vanillapdf/branch/main/graph/badge.svg?token=1UO4W5XGTL)](https://codecov.io/gh/vanillapdf/vanillapdf) [![Fuzzing](https://github.com/vanillapdf/vanillapdf/actions/workflows/fuzzing.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/fuzzing.yml) [![CodeQL](https://github.com/vanillapdf/vanillapdf/actions/workflows/codeql.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/codeql.yml) [![Sanitizers](https://github.com/vanillapdf/vanillapdf/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/sanitizers.yml) | [![OpenSSF Scorecard](https://api.scorecard.dev/projects/github.com/vanillapdf/vanillapdf/badge)](https://scorecard.dev/viewer/?uri=github.com/vanillapdf/vanillapdf) [![SLSA 2](https://slsa.dev/images/gh-badge-level2.svg)](https://slsa.dev) |

</div>

**Vanilla.PDF** is an open-source C++17 library with a plain C API, usable across multiple languages through a C foreign function interface (FFI). It is licensed under the permissive Apache 2.0 license, suitable for both private and commercial use.

> **Using .NET?** See [vanillapdf.net](https://github.com/vanillapdf/vanillapdf.net), the managed .NET wrapper with its own NuGet package and samples. This repository covers the native C API.

- 📄 **Create & modify** — pages, text, images, vector graphics, form fields
- 🔏 **Sign & verify** — CMS/PKCS#7 signatures with certificate chain validation
- 🔒 **Encrypt & decrypt** — AES or RC4, with passwords or certificates
- ⚙️ **Stable ABI** — no breaking changes within a major version
- 🧵 **Thread-safe** — share any object across threads; synchronization is handled internally
- 🛡️ **Hardened** — fuzzed and tested under ASan, UBSan and TSan every week

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

- **Build from source** — clone with submodules, then configure with a CMake preset

🚧 **In progress:** Conan Center and Homebrew packages are not published yet.

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

See the [quickstart guide](https://vanillapdf.readthedocs.io/en/latest/quickstart.html) for the full example with error handling and the `CMakeLists.txt` to build it. The source is in [`examples/create/`](examples/create/), which the test suite builds and runs.

## Platforms

Every platform below is built and tested by the nightly CI:

| Platform | Versions | Compilers | Architectures |
|----------|----------|-----------|---------------|
| Windows | Windows Server 2022, 2025; Windows 11 | Visual Studio 2026, 2022 | x86, x64, ARM64 |
| Ubuntu | 22.04, 24.04, 26.04 | GCC 11, 13, 15; Clang 21 | x64, ARM64 |
| Rocky Linux | 8, 9, 10 | GCC 8, 11, 14 | x64, ARM64 |
| Fedora | 44, 45 | GCC 16 | x64, ARM64 |
| macOS | 15, 26, 27 | AppleClang 17+ (Xcode 16.4+) | x64, ARM64 |
| Android | API level 28, 36 | NDK r28+ | x86_64 |

See [Supported platforms](https://vanillapdf.readthedocs.io/en/latest/overview.html#supported-platforms) for the exceptions per architecture.

## Features

| Feature | C API | Description |
|---------|-------|-------------|
| **Create documents** | `Document_Create` | Pages, text, images, vector paths |
| **Digital signatures** | `Document_Sign` | OpenSSL CMS — RSA, ECDSA (P-256/P-384/P-521), Ed25519 (OpenSSL 3.2+) via PKCS#12 keys or custom callbacks |
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
| [Installation](https://vanillapdf.readthedocs.io/en/latest/installation.html) | vcpkg, FetchContent |
| [Building](https://vanillapdf.readthedocs.io/en/latest/building.html) | Build from source with CMake presets |
| [Quickstart](https://vanillapdf.readthedocs.io/en/latest/quickstart.html) | Create your first PDF document step by step |
| [Architecture](https://vanillapdf.readthedocs.io/en/latest/architecture.html) | Three-layer design, object model, memory model, thread safety |
| [C API Guide](https://vanillapdf.readthedocs.io/en/latest/c_api.html) | Handles, memory management, error handling |
| [Build Options](https://vanillapdf.readthedocs.io/en/latest/build_options.html) | Dependencies, feature and development options |
| [Testing](https://vanillapdf.readthedocs.io/en/latest/testing.html) | Tests, test data, fuzzing and coverage |
| [PDF Format](https://vanillapdf.readthedocs.io/en/latest/pdf_format.html) | PDF syntax, objects, and document structure |
| [Signature Verification](https://vanillapdf.readthedocs.io/en/latest/signature_verification.html) | Trust stores, chain validation, weak-algorithm detection |
| [Examples](https://vanillapdf.readthedocs.io/en/latest/examples.html) | Tested programs for merging, signing, verification, encryption and validation |

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
