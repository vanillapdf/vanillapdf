#!/usr/bin/env bash

# Pushes everything the Android test run needs onto the connected device or
# emulator, mirroring the host repository layout under the device root so the
# adb_test_runner.sh path translation stays a simple prefix replacement.
#
# Expects the android-x86_64 preset to be built and a device visible to adb.
#
# Environment:
#   VANILLAPDF_ANDROID_HOST_ROOT   - repository root on the host (required)
#   VANILLAPDF_ANDROID_DEVICE_ROOT - device destination root
#                                    (default /data/local/tmp/vanillapdf)

set -euo pipefail

host_root="${VANILLAPDF_ANDROID_HOST_ROOT:?VANILLAPDF_ANDROID_HOST_ROOT is not set}"
device_root="${VANILLAPDF_ANDROID_DEVICE_ROOT:-/data/local/tmp/vanillapdf}"
build_directory="$host_root/build/android-x86_64"

# Mirror a host file or directory to the device, preserving the path relative
# to the repository root
push() {
    local host_path="$1"
    local relative_path="${host_path#"$host_root"/}"
    local device_path="$device_root/$relative_path"

    adb shell mkdir -p "$(dirname "$device_path")"
    adb push "$host_path" "$device_path"
}

push "$build_directory/src/vanillapdf/libvanillapdf.so"
push "$build_directory/src/vanillapdf.test/vanillapdf.test"
push "$build_directory/src/vanillapdf.tools/vanillapdf.tools"
push "$build_directory/src/vanillapdf.unittest/vanillapdf.unittest"
push "$build_directory/_deps/vanillapdf_testdata-src"
push "$host_root/scripts/VanillaPDF.lic"
push "$host_root/fixtures"

adb shell chmod +x \
    "$device_root/build/android-x86_64/src/vanillapdf.test/vanillapdf.test" \
    "$device_root/build/android-x86_64/src/vanillapdf.tools/vanillapdf.tools" \
    "$device_root/build/android-x86_64/src/vanillapdf.unittest/vanillapdf.unittest"
