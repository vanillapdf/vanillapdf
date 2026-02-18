# Android NDK Integration Example

This example demonstrates how to cross-compile `vanillapdf` for Android ABIs
using the Android NDK toolchain through vcpkg chainloading.

## Prerequisites

- Android NDK installed (r26+ recommended)
- `ANDROID_NDK_ROOT` environment variable set to your NDK path
- vcpkg submodule initialized from the repository root:

```bash
git submodule sync --recursive
git submodule update --init --recursive
```

## Build

From this directory:

```bash
cmake --preset android-arm64-release
cmake --build --preset android-arm64-release
```

Available presets:

- `android-arm64-release` (`arm64-v8a`, RID-compatible with `android-arm64`)
- `android-arm-release` (`armeabi-v7a`, RID-compatible with `android-arm`)
- `android-x64-release` (`x86_64`, RID-compatible with `android-x64`)

By default, this example disables encryption/image/compression features to keep
cross-compilation CI fast. Enable full feature coverage with:

```bash
cmake --preset android-arm64-release -DANDROID_NDK_EXAMPLE_FULL_FEATURES=ON
cmake --build --preset android-arm64-release
```

This example is a build-only smoke test for cross-compilation and does not run
the produced binaries on the host machine.
