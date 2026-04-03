# Vanilla.PDF

**Vanilla.PDF** is a modern, high-performance C++17 library for creating, editing, and digitally signing PDF documents. It is cross-platform and runs natively on Windows, Linux, and macOS — without requiring external dependencies or a runtime.

## Features

- Create or modify PDF files programmatically
- Add or validate digital signatures (PAdES)
- Encrypt and decrypt PDF files
- Access low-level PDF objects and structure directly
- Fast and lightweight native runtime for each platform

## Supported Platforms & Compilers

- **Windows:** Visual Studio 2022 (MSVC 17.x) or 2026 (MSVC 18.x) — x86, x64, ARM64
- **Linux:** GCC 8.1+ or Clang 10+ (x64, ARM64, ARM)
- **macOS:** AppleClang 15+ (Xcode 15)
- **Android:** NDK toolchain (arm64-v8a, armeabi-v7a, x86, x86_64)

## Getting Started

1. Install the NuGet package:
   ```
   dotnet add package vanillapdf.net
   ```
2. The correct native runtime will be included automatically based on your target platform and RID.
3. Refer to our [documentation](https://vanillapdf.github.io/vanillapdf) for usage examples and integration details.

## Resources

- 🔧 [GitHub Repository](https://github.com/vanillapdf/vanillapdf)
- 📘 [Documentation](https://vanillapdf.github.io/vanillapdf)
- 💬 [FAQ](https://vanillapdf.com/faq/)
- 📨 [Contact](https://vanillapdf.com/contact/)

## License

Licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

For changes and release notes, see the [GitHub Releases](https://github.com/vanillapdf/vanillapdf/releases).
