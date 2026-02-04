# Vanilla.PDF

[![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml)
[![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue)](https://www.apache.org/licenses/LICENSE-2.0)

**Vanilla.PDF** is a cross-platform C library for creating, signing, encrypting, and parsing PDF documents. It compiles natively on Windows, Linux, macOS, and Android with no external runtime dependencies.

## Features

- Create or modify PDF files programmatically
- Add or validate digital signatures (CMS/PKCS#7)
- Encrypt and decrypt PDF files (AES, RC4)
- Access low-level PDF objects and structure directly
- Lightweight native runtime for each platform

## Quick Example

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

## Supported Platforms

| Platform | Compilers | Architectures |
|----------|-----------|---------------|
| Windows | Visual Studio 2022, 2026 | x86, x64 |
| Linux | GCC 8.1+, Clang 10+ | x64, ARM64, ARM |
| macOS | AppleClang 15+ (Xcode 15) | x64, ARM64 |
| Android | NDK toolchain | arm64, armv7, x86, x86_64 |

## Getting Started

1. Install the NuGet package:
   ```
   dotnet add package vanillapdf.net
   ```
2. The correct native runtime will be included automatically based on your target platform and RID.
3. Refer to the [documentation](https://vanillapdf.readthedocs.io/) for usage examples and integration details.

## Resources

- [GitHub Repository](https://github.com/vanillapdf/vanillapdf)
- [Documentation](https://vanillapdf.readthedocs.io/)
- [FAQ](https://vanillapdf.com/faq/)
- [Contact](https://vanillapdf.com/contact/)

## License

Licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

For changes and release notes, see the [GitHub Releases](https://github.com/vanillapdf/vanillapdf/releases).
