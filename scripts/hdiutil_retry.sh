#!/bin/bash

# Retry script for hdiutil to work around XProtect race conditions on macOS 13+
# Based on workaround from: https://github.com/HEXRD/hexrdgui/pull/1768
# Related: https://gitlab.kitware.com/cmake/cmake/-/issues/25671

COMMAND="$1"

# For detach, escalate to -force after gentle attempts fail
if [ "$COMMAND" = "detach" ]; then
    for i in {1..5}; do
        if /usr/bin/hdiutil "$@"; then
            exit 0
        fi
        echo "hdiutil detach failed, attempt $i/10" >&2
        sleep 3
    done

    # Escalate to force detach
    for i in {6..10}; do
        if /usr/bin/hdiutil "$@" -force; then
            exit 0
        fi
        echo "hdiutil detach -force failed, attempt $i/10" >&2
        sleep 3
    done

    echo "hdiutil detach failed after 10 attempts" >&2
    exit 1
fi

# For all other commands (create, attach, etc.) retry normally
for i in {1..10}; do
    if /usr/bin/hdiutil "$@"; then
        exit 0
    fi
    echo "hdiutil $COMMAND failed, attempt $i/10" >&2
    sleep 2
done

echo "hdiutil $COMMAND failed after 10 attempts" >&2
exit 1
