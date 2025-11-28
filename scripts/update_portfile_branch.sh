#!/bin/bash
# Update portfile.cmake to use a specific branch for HEAD_REF
# Usage: ./update_portfile_branch.sh <branch_name>

set -e

BRANCH="${1:-main}"
PORTFILE="ports/vanillapdf/portfile.cmake"

if [ ! -f "$PORTFILE" ]; then
    echo "Error: $PORTFILE not found"
    exit 1
fi

echo "Updating HEAD_REF to: $BRANCH"

# Use different sed syntax for macOS vs Linux
if [[ "$OSTYPE" == "darwin"* ]]; then
    sed -i '' "s|HEAD_REF main|HEAD_REF $BRANCH|" "$PORTFILE"
else
    sed -i "s|HEAD_REF main|HEAD_REF $BRANCH|" "$PORTFILE"
fi

echo "Updated portfile:"
grep "HEAD_REF" "$PORTFILE"
