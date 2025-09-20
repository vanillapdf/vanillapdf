# Vanilla.PDF – The Ultimate C++ PDF Toolkit

[![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![Downloads](https://img.shields.io/nuget/dt/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue)](LICENSE.txt)
[![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml)
[![Coverage](https://codecov.io/gh/vanillapdf/vanillapdf/branch/main/graph/badge.svg?token=1UO4W5XGTL)](https://codecov.io/gh/vanillapdf/vanillapdf)
[![Docs](https://img.shields.io/badge/docs-available-brightgreen)](https://vanillapdf.github.io/vanillapdf)
[![Repo Size](https://img.shields.io/github/repo-size/vanillapdf/vanillapdf)](https://github.com/vanillapdf/vanillapdf)

**Vanilla.PDF** is a modern, high-performance, open-source C++17 SDK for creating, editing, signing, and analyzing PDF documents. With no external runtime dependencies and full cross-platform support, it’s ideal for embedding into desktop, server, or automation workflows.

---

## 🚀 Features

- ✅ **Digital Signatures**: Add CMS (PKCS#7) digital signatures to existing PDFs
- 📄 **PDF Generation**: Create documents with text, images, paths, and pages
- 🛠️ **Advanced Editing**: Modify content streams, metadata, and page structure
- 🔍 **PDF Parsing**: Inspect low-level internals like XRef tables and objects
- 🔐 **Encryption & Permissions**: Work with standard PDF security models
- 🧪 **Test Coverage**: Unit-tested core with CI pipelines and sanitizers (enable stack sanitizer in Debug builds with `-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON`)
- 🧰 **CLI Tools**: Batch-process PDFs directly from the terminal
- ⚙️ **Cross-Platform**: Build on Windows, Linux, macOS, and Android via CMakePresets
- 📦 **Minimal Dependencies**: Statically linkable; vcpkg and Conan compatible
- ⚡ **High Performance**: Optimized C++17 core with benchmark testing suite
- 🔄 **CI/CD**: Automated testing across all platforms with nightly builds

---

## 🖥️ Supported Platforms & Compilers

- **Windows:** Visual Studio 2019 (MSVC 16.11) or 2022 (MSVC 17.x)
- **Linux:** GCC 8.1+ or Clang 10+ (x64, ARM64, ARM)
- **macOS:** AppleClang 15+ (Xcode 15)
- **Android:** NDK toolchain (arm64-v8a, armeabi-v7a, x86, x86_64)

---

## 🚀 Quick Start

```bash
# Clone and build in 3 steps (Windows example)
git clone https://github.com/vanillapdf/vanillapdf.git
cd vanillapdf && git submodule update --init --recursive
cmake --preset windows-x64-msvc-17 && cmake --build --preset windows-x64-msvc-17

# Linux/macOS: use linux-x64-gcc or macos-arm64 preset instead
```

## 📦 Installation Options

Detailed setup instructions are available in the
[installation guide](https://vanillapdf.github.io/vanillapdf/page_install.html).

### 🛠 Build from Source

```bash
git clone https://github.com/vanillapdf/vanillapdf.git
cd vanillapdf

# Fetch the bundled vcpkg submodule (skip if VCPKG_ROOT points to an existing install)
git submodule sync --recursive
git submodule update --init --recursive
```

To see which build presets are available run:

```bash
cmake --list-presets
```

```
Available configure presets:
  "windows-x86-ninja"
  "windows-x64-ninja"
  "windows-x86-msvc-17"
  "windows-x64-msvc-17"
  "windows-x86-msvc-16"
  "windows-x64-msvc-16"
  "linux-x64-gcc"
  "linux-arm64-gcc"
  "linux-x64-clang"
  "linux-arm64-clang"
  "linux-x64-musl"
  "linux-arm64-musl"
  "linux-arm-musl"
  "macos-x64"
  "macos-arm64"
  "android-arm64"
  "android-armv7"
  "android-x86"
  "android-x86_64"
```

Choose the preset matching your platform:

```bash
cmake --preset windows-x64-msvc-17
cmake --build --preset windows-x64-msvc-17
ctest --preset windows-x64-msvc-17  # Optional
```

The repository includes `external/vcpkg` as a submodule. Initializing the
submodule is required unless you provide `VCPKG_ROOT` or pass your own
`-DCMAKE_TOOLCHAIN_FILE` pointing to a preinstalled vcpkg.

### Install Dependencies with vcpkg

If you prefer to use [vcpkg](https://github.com/microsoft/vcpkg) for dependency
management, bootstrap it and install the required packages:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install openssl libjpeg-turbo
```

Then configure Vanilla.PDF with the vcpkg toolchain:

```bash
cmake -S . -B build/vcpkg \
  -DCMAKE_TOOLCHAIN_FILE=</path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build/vcpkg
```

### Using System Dependencies

If some libraries should come from your package manager or Conan rather than
vcpkg, enable the matching `VANILLAPDF_EXTERNAL_*` options when configuring:

```bash
cmake -B build \
  -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
  -DVANILLAPDF_EXTERNAL_JPEG=ON
cmake --build build
```

Options exist for OpenSSL, libjpeg-turbo, openjpeg, zlib, spdlog,
nlohmann-json, GTest and Google Benchmark. When these are set, the
corresponding packages will not be installed by vcpkg and CMake will
search for them on your system instead. The GTest and Benchmark options
are only available when tests or benchmarks are enabled.

For instructions on generating Debian or Homebrew packages see the
[packaging guide](https://vanillapdf.github.io/vanillapdf/page_packaging.html).

---

## 🔗 CMake Integration

### Using vcpkg (Recommended)

The fastest way to integrate VanillaPDF is using vcpkg for optimal build performance:

```cmake
find_package(vanillapdf CONFIG REQUIRED)

target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

This requires Vanilla.PDF to be installed via vcpkg:
```bash
vcpkg install vanillapdf
```

**Benefits:**
- ⚡ **Fastest builds**: Pre-compiled binaries, no source compilation during build
- 🔧 **Feature control**: Enable specific features with `vcpkg install vanillapdf[openssl,jpeg,jpeg2000]`
- 📦 **Dependency management**: Handles all dependencies automatically
- 🔄 **Caching**: Shared binary cache across projects
- 🏢 **Enterprise-ready**: Proven at scale in production environments

### Using FetchContent (Alternative)

For users who prefer not to introduce external tools into their build chain:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyApp)

include(FetchContent)
FetchContent_Declare(
    vanillapdf
    GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
    GIT_TAG        main  # or "v2.1.0" for specific release
)
FetchContent_MakeAvailable(vanillapdf)

# Link to your target
target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

**Benefits:**
- ✅ **Self-contained**: No external dependency managers required
- ✅ **Auto-detection**: Automatically detects embedded mode (no packaging conflicts)
- ✅ **Cross-platform**: Tested on Windows (vcpkg), Linux (external), macOS (Homebrew)
- ✅ **Feature control**: Configure optional features via CMake options
- ✅ **Simple setup**: Default features enabled (encryption, JPEG, JPEG2000)

> **⚠️ Note**: When using FetchContent, you are responsible for managing VanillaPDF's dependencies. This can be done through system package managers (`apt-get install libssl-dev libjpeg-turbo8-dev` on Linux, `brew install openssl libjpeg-turbo` on macOS) or other packaging systems like Conan.

> **✅ Integration Status**: [![FetchContent Integration](https://github.com/vanillapdf/vanillapdf/actions/workflows/examples/fetchcontent-integration.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/examples/fetchcontent-integration.yml)

### Using Manual Installation

```cmake
find_package(vanillapdf CONFIG REQUIRED)

target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

This requires Vanilla.PDF to be installed manually via:
- `cmake --install` from a built source tree
- Setting `CMAKE_PREFIX_PATH` to the install location

---

## 📋 Examples

### FetchContent Integration Example

See [`examples/fetchcontent-integration/`](examples/fetchcontent-integration/) for a complete working example that demonstrates real-world FetchContent usage:

**Features demonstrated:**
- ✅ **Self-contained integration** - No external dependency managers required
- ✅ **Cross-platform presets** - Windows (x64), Linux (x64), macOS (ARM64)
- ✅ **Dependency flexibility** - Choose between vcpkg or external system dependencies
- ✅ **Real PDF creation** - Creates actual PDF files with VanillaPDF functionality
- ✅ **Feature configuration** - CMake options to enable/disable features (OpenSSL, JPEG, etc.)

**Quick start:**
```bash
cd examples/fetchcontent-integration
cmake --preset windows-x64-debug    # Windows
cmake --preset linux-x64-debug      # Linux
cmake --preset macos-arm64-debug    # macOS
cmake --build --preset [preset-name]
```

**Continuous validation**: This integration approach is continuously tested across all platforms - see the [workflow results](https://github.com/vanillapdf/vanillapdf/actions/workflows/examples/fetchcontent-integration.yml) for live status.

---

## 🧰 CLI Utility: `vanillapdf-tools`

Use the bundled CLI tool for quick PDF tasks like signing:

```bash
./vanillapdf-tools <command> [options]
```

### Example: Sign a PDF

```bash
./vanillapdf-tools sign \
  -s input.pdf \
  -d signed.pdf \
  -k private_key.p12 \
  -p your_password
```

| Option | Description |
|--------|-------------|
| `-s`   | Source PDF file |
| `-d`   | Output PDF file |
| `-k`   | Private key (PKCS#12) |
| `-p`   | Password for the key |

Run `--help` for a complete list of supported commands.

---

## 📚 Documentation

- 📖 [Official Documentation](https://vanillapdf.github.io/vanillapdf/)
- 📐 [API Reference](https://vanillapdf.github.io/vanillapdf/annotated.html) (Doxygen)
- 🛠️ [Installation Guide](https://vanillapdf.github.io/vanillapdf/page_install.html)
- 📦 [Packaging Guide](https://vanillapdf.github.io/vanillapdf/page_packaging.html)
- 🧩 [C++ Object Model Overview](https://vanillapdf.github.io/vanillapdf/page_architecture.html)
- 📊 [Performance Benchmarks](https://vanillapdf.github.io/vanillapdf/page_benchmarks.html)

---

## 🤝 Contributing

We welcome pull requests, feature proposals, and bug reports! All contributions must follow our branching workflow.

- 📜 [Code of Conduct](CODE_OF_CONDUCT.md)
- 🛠️ [Contributing Guidelines](CONTRIBUTING.md)
- 🔧 [Development Setup](CLAUDE.md) (for maintainers)
- 🐛 [Issue Templates](.github/ISSUE_TEMPLATE/)

**Development Process:**
- All changes require a branch and pull request (main/release branches are protected)
- Automated testing runs on Windows, Linux, macOS, and Android
- Code coverage and sanitizer testing ensure quality

---

## 🔒 License

Vanilla.PDF is licensed under the [Apache 2.0 License](LICENSE.txt).

For a complete list of changes, see the [Releases page](https://github.com/vanillapdf/vanillapdf/releases).

---

## 💬 Contact & Support

📧 info@vanillapdf.com  
🌐 [vanillapdf.com/contact](https://vanillapdf.com/contact/)  
🐙 [GitHub Issues](https://github.com/vanillapdf/vanillapdf/issues)

---

## ❤️ Support Us

If you find Vanilla.PDF useful:

- ⭐ Star the project
- 🔗 Share it with your network
- ☕ [Sponsor development](https://github.com/sponsors/vanillapdf)
