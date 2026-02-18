# RID Build Container Specification

This document defines the recommended prebuilt Docker images for runtime
packages that target .NET RID-compatible names.

## Scope

The current RID expansion requires dedicated build environments for:

- `linux-musl-x64`
- `linux-musl-arm64`
- `android-arm64`
- `android-arm`
- `android-x64`
- `linux-bionic-arm64`
- `linux-bionic-arm`
- `linux-bionic-x64`

## Recommended Container Images

### 1) Linux musl images

#### `linux-musl-x64`

- **Source image:** `alpine:3.21` (linux/amd64)
- **Suggested target image tag:** `ghcr.io/vanillapdf/vanillapdf-alpine-amd64:3.21-musl-rid`
- **Preinstalled software:**
  - `bash`
  - `cmake` (3.28+)
  - `ninja`
  - `gcc`, `g++`, `make`
  - `musl-dev`, `linux-headers`
  - `git`
  - `curl`
  - `python3`
  - `zip`, `unzip`, `tar`
  - `pkgconf`
  - `ca-certificates`
  - `icu-libs` (for `dotnet`)
  - `.NET SDK 10.x` (if packing in-container)

#### `linux-musl-arm64`

- **Source image:** `alpine:3.21` (linux/arm64)
- **Suggested target image tag:** `ghcr.io/vanillapdf/vanillapdf-alpine-arm64v8:3.21-musl-rid`
- **Preinstalled software:** same as `linux-musl-x64`.

### 2) Android / bionic cross-build image

#### `android-*` and `linux-bionic-*` runtime packages

- **Source image:** `ubuntu:24.04` (linux/amd64)
- **Suggested target image tag:** `ghcr.io/vanillapdf/vanillapdf-android-ndk-amd64:r27c`
- **Preinstalled software:**
  - `bash`
  - `cmake` (3.28+)
  - `ninja-build`
  - `build-essential`
  - `git`
  - `curl`
  - `python3`
  - `zip`, `unzip`, `tar`
  - `pkg-config`
  - `openjdk-17-jdk` (Android SDK tooling requirement)
  - Android command-line tools (`sdkmanager`)
  - Android NDK (`r27c` recommended)
  - `.NET SDK 10.x`
  - `icu` runtime libraries (for `dotnet`)

### Required environment variables

Set these in the image:

- `ANDROID_SDK_ROOT` (for example `/opt/android-sdk`)
- `ANDROID_NDK_ROOT` (for example `/opt/android-sdk/ndk/27.2.12479018`)

## RID to ABI Mapping

| RID | Android ABI | vcpkg triplet |
| --- | --- | --- |
| `android-arm64` | `arm64-v8a` | `arm64-android` |
| `android-arm` | `armeabi-v7a` | `arm-neon-android` |
| `android-x64` | `x86_64` | `x64-android` |
| `linux-bionic-arm64` | `arm64-v8a` | `arm64-android` |
| `linux-bionic-arm` | `armeabi-v7a` | `arm-neon-android` |
| `linux-bionic-x64` | `x86_64` | `x64-android` |

`linux-bionic-*` packages are compatibility variants built from the same
Android toolchain output as the corresponding `android-*` packages.
