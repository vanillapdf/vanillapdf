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
- 🧪 **Test Coverage**: Unit-tested core with CI pipelines and sanitizers
- 🧰 **CLI Tools**: Batch-process PDFs directly from the terminal
- ⚙️ **Cross-Platform**: Build on Windows, Linux, and macOS via CMakePresets
- 📦 **Minimal Dependencies**: Statically linkable; vcpkg and Conan compatible

---

## 🖥️ Supported Platforms & Compilers

- **Windows:** Visual Studio 2019 (MSVC 16.11) or 2022 (MSVC 17.x)
- **Linux:** GCC 8.1+ or Clang 10+ (x64, ARM64, ARM)
- **macOS:** AppleClang 15+ (Xcode 15)
- **Android:** NDK toolchain (arm64-v8a, armeabi-v7a, x86, x86_64)

---

## 📦 Installation Options

### 🛠 Build from Source

```bash
git clone https://github.com/vanillapdf/vanillapdf.git
cd vanillapdf
cmake --preset windows-x64-msvc-release
cmake --build --preset windows-x64-msvc-release
ctest --preset windows-x64-msvc-release  # Optional
```

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

### Building Debian Packages

On Ubuntu systems you can install all required dependencies with apt and
produce a `.deb` package directly:

```bash
sudo apt-get update
sudo apt-get install libssl-dev libjpeg-turbo8 libjpeg-turbo8-dev zlib1g-dev libopenjp2-7-dev \
  libspdlog-dev nlohmann-json3-dev libgtest-dev libbenchmark-dev

cmake -S . -B build/deb -DCMAKE_BUILD_TYPE=Release \
  -DVANILLAPDF_STANDALONE=OFF \
  -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
  -DVANILLAPDF_EXTERNAL_JPEG=ON \
  -DVANILLAPDF_EXTERNAL_OPENJPEG=ON \
  -DVANILLAPDF_EXTERNAL_ZLIB=ON \
  -DVANILLAPDF_EXTERNAL_SPDLOG=ON \
  -DVANILLAPDF_EXTERNAL_NLOHMANN_JSON=ON \
  -DVANILLAPDF_ENABLE_TESTS=ON -DVANILLAPDF_EXTERNAL_GTEST=ON \
  -DVANILLAPDF_ENABLE_BENCHMARK=ON -DVANILLAPDF_EXTERNAL_BENCHMARK=ON
cmake --build build/deb --target package
```

---

## 🔗 CMake Integration (Modern Target-Based)

```cmake
find_package(vanillapdf CONFIG REQUIRED)

target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

This requires Vanilla.PDF to be installed properly via:
- `cmake --install`
- vcpkg (`vcpkg install vanillapdf`)
- or manually setting `CMAKE_PREFIX_PATH` to the install location

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

- 📖 [Official Docs](https://vanillapdf.github.io/)
- 📐 API Reference (Doxygen)
- 🧩 C++ object model overview
- 🐍 Python and .NET binding guides

---

## 🤝 Contributing

We welcome pull requests, feature proposals, and bug reports!

- 📜 [Code of Conduct](CODE_OF_CONDUCT.md)
- 🛠️ `CONTRIBUTING.md` (coming soon)

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
- ☕ [Sponsor development](https://github.com/sponsors/vanillapdf) (coming soon)
