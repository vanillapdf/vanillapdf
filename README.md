# Vanilla.PDF

[![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![Downloads](https://img.shields.io/nuget/dt/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue)](LICENSE.txt)
[![Build Status](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml)
[![Repo Size](https://img.shields.io/github/repo-size/vanillapdf/vanillapdf)](https://github.com/vanillapdf/vanillapdf)
[![Docs](https://img.shields.io/badge/docs-available-brightgreen)](https://vanillapdf.github.io/)

**Vanilla.PDF** is a modern, high-performance C++ SDK for creating, signing, and modifying PDF documents. Built with efficiency and simplicity in mind, it empowers developers with fine-grained control over PDF internals while maintaining an intuitive API and cross-platform compatibility.

---

## ✨ Highlights

- ✅ Digitally sign PDF documents with ease
- ✍️ Modify existing PDFs: metadata, structure, contents
- 📄 Generate new PDFs with custom pages, text, images, and shapes
- 🛠️ Tweak PDF internals for advanced use cases
- 📦 Command-line utility for quick operations (`vanillapdf-tools`)
- 🚫 No external runtime dependencies
- ⚙️ Cross-platform binaries for Windows, Linux, and macOS

---

## 📖 About

Vanilla.PDF is a compiled C++ library offering deep PDF manipulation functionality. Whether you're adding a signature, inspecting metadata, tweaking page structure, or generating a fresh document — Vanilla.PDF provides the performance and flexibility needed.

It’s lightweight, blazingly fast, and built for integration:
- Prebuilt binaries available for major platforms
- Minimal footprint, no bloated dependencies
- Comprehensive documentation and tested build process

🔗 [Official Website](http://vanillapdf.com)
📚 [Documentation](https://vanillapdf.github.io/)

---

## 🚀 Getting Started

### 🔧 Build from Source

```bash
git clone https://github.com/vanillapdf/vanillapdf.git
cd vanillapdf

# List available build presets
cmake --list-presets

# Configure and build using MSVC 64-bit release preset
cmake --preset windows-x64-msvc-release
cmake --build --preset windows-x64-msvc-release

# Optional: Run tests
ctest --preset windows-x64-msvc-release
```

### 📦 CMake Integration

```cmake
target_include_directories(myapp PRIVATE path/to/vanillapdf/include)
target_link_libraries(myapp PRIVATE vanillapdf)
```

---

## 🛠️ Command-line Utility: `vanillapdf-tools`

Vanilla.PDF includes a powerful CLI tool for performing PDF operations directly from the terminal.

### 🔍 Usage

```bash
./vanillapdf-tools <command> [options]
```

### 🧰 Commands

- `sign` Digitally sign a PDF

### 🔑 Common Options

| Option | Description |
|--------|-------------|
| `-s`   | Path to source PDF file |
| `-d`   | Destination/output PDF file |
| `-k`   | Private key file (PKCS#12) |
| `-p`   | Password for the key |

Use `./vanillapdf-tools --help` for full command reference.

### ✅ Example: Sign a PDF

```bash
./vanillapdf-tools sign -s input.pdf -d signed.pdf -k private_key.p12 -p your_password
```

---

## 🔒 License

Vanilla.PDF is released under the [Apache 2.0 License](LICENSE.txt).

---

## 🤝 Contributions

We welcome contributions, feature requests, and bug reports. A [contributing guide](CONTRIBUTING.md) is coming soon. Stay tuned!

---

## 📬 Contact

Questions or support needs?  
📧 [info@vanillapdf.com](mailto:info@vanillapdf.com)
🌐 [vanillapdf.com/contact](https://vanillapdf.com/contact/)

## 🧭 Code of Conduct

We are committed to fostering a welcoming and respectful environment.
Please read our [Code of Conduct](CODE_OF_CONDUCT.md) before participating.
