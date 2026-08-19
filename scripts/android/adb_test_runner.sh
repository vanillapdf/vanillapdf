#!/usr/bin/env bash

# CMake CROSSCOMPILING_EMULATOR wrapper for Android test execution.
#
# ctest (and scripts/run_test.py via its --launcher option) invokes this script
# as: adb_test_runner.sh <host-binary-path> [arguments...]. The binaries and
# their data files must already be on the device, mirroring their layout
# relative to the repository root — scripts/android/push_test_payload.sh does
# that. Every argument that starts with the host root is rewritten to the
# device root; everything else passes through unchanged. The command then runs
# on the device through adb shell, which propagates stdout and the exit code.
#
# Environment:
#   VANILLAPDF_ANDROID_HOST_ROOT   - repository root on the host (required)
#   VANILLAPDF_ANDROID_DEVICE_ROOT - device destination root
#                                    (default /data/local/tmp/vanillapdf)
#   VANILLAPDF_ANDROID_LIBRARY_DIR - host directory containing libvanillapdf.so,
#                                    exported as LD_LIBRARY_PATH after the same
#                                    prefix translation (optional)

set -euo pipefail

host_root="${VANILLAPDF_ANDROID_HOST_ROOT:?VANILLAPDF_ANDROID_HOST_ROOT is not set}"
device_root="${VANILLAPDF_ANDROID_DEVICE_ROOT:-/data/local/tmp/vanillapdf}"
library_directory="${VANILLAPDF_ANDROID_LIBRARY_DIR:-}"

translated_arguments=()
for argument in "$@"; do
    translated_arguments+=("${argument/#"$host_root"/$device_root}")
done

environment_prefix=""
if [[ -n "$library_directory" ]]; then
    environment_prefix="LD_LIBRARY_PATH=${library_directory/#"$host_root"/$device_root} "
fi

# %q quoting keeps arguments with spaces or non-ASCII characters intact across
# the adb shell boundary
printf -v device_command '%q ' "${translated_arguments[@]}"

exec adb shell "${environment_prefix}${device_command}"
