#!/bin/bash

# Retry script for hdiutil create to work around XProtect race conditions
# Based on workaround from: https://github.com/HEXRD/hexrdgui/pull/1768

for i in {1..10}; do
    if hdiutil create "$@"; then
        exit 0
    fi
    echo "hdiutil create failed, attempt $i/10"
    sleep 1
done

echo "hdiutil create failed after 10 attempts"
exit 1