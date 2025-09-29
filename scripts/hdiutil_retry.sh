#!/bin/bash

# Retry script for hdiutil to work around XProtect race conditions on macOS 13+
# Based on workaround from: https://github.com/HEXRD/hexrdgui/pull/1768
# Related: https://gitlab.kitware.com/cmake/cmake/-/issues/25671

for i in {1..10}; do
    if /usr/bin/hdiutil "$@"; then
        exit 0
    fi
    echo "hdiutil $1 failed, attempt $i/10" >&2
    sleep 2
done

echo "hdiutil $1 failed after 10 attempts" >&2
exit 1