# Vanilla.PDF – The Ultimate C++ PDF Toolkit

[![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml)
[![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![NuGet Downloads](https://img.shields.io/nuget/dt/vanillapdf)](https://www.nuget.org/packages/vanillapdf)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue)](https://www.apache.org/licenses/LICENSE-2.0)

**Vanilla.PDF** is a modern, high-performance, open-source C++17 SDK for creating, editing, signing, and analyzing PDF documents. The NuGet package provides native runtime binaries for Windows, Linux, and macOS — automatically selected based on your target platform and RID. No manual DLL management required.

- 📄 **Create & modify** PDF documents — pages, text, images, vector graphics
- 🔏 **Sign & verify** digital signatures (CMS/PKCS#7) with certificate chain validation
- 🔒 **Encrypt & decrypt** with AES or RC4 using passwords or certificates
- ⚙️ **ABI-stable C API** — callable via P/Invoke from C#, F#, or any .NET language
- 🧵 **Thread-safe** — no global state, process documents in parallel without locking

## Getting Started

Install the NuGet package:

```
dotnet add package vanillapdf.net
```

The correct native runtime is included automatically. No additional setup is needed — the package handles native binary selection and deployment for your target platform.

Refer to the [documentation](https://vanillapdf.readthedocs.io/) for usage examples and integration details.

## Supported Platforms

The package ships native binaries for the following runtime identifiers:

| Platform | RID | Architecture |
|----------|-----|--------------|
| Windows | `win-x64` | x64 |
| Windows | `win-x86` | x86 |
| Windows | `win-arm64` | ARM64 |
| Linux | `linux-x64` | x64 |
| Linux | `linux-arm64` | ARM64 |
| macOS | `osx-x64` | x64 (Intel) |
| macOS | `osx-arm64` | ARM64 (Apple Silicon) |

## Resources

- [GitHub Repository](https://github.com/vanillapdf/vanillapdf)
- [Documentation](https://vanillapdf.readthedocs.io/)
- [FAQ](https://vanillapdf.com/faq/)
- [Contact](https://vanillapdf.com/contact/)

## License

Licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

For changes and release notes, see the [GitHub Releases](https://github.com/vanillapdf/vanillapdf/releases).
