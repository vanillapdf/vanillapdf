#!/bin/bash
# Update portfile.cmake to use a specific branch for HEAD_REF
# Usage: ./update_portfile_branch.sh <branch_name>
#
# If GITHUB_TOKEN is set, an AUTHORIZATION_TOKEN reference is also added so
# vcpkg's GitHub API call resolving the branch head (--head mode) is
# authenticated. Anonymous API requests share a per-IP rate limit, which
# shared CI runners (especially macOS) frequently exhaust, causing 403s.

set -e

BRANCH="${1:-main}"
PORTFILE="ports/vanillapdf/portfile.cmake"

if [ ! -f "$PORTFILE" ]; then
    echo "Error: $PORTFILE not found"
    exit 1
fi

echo "Updating HEAD_REF to: $BRANCH"

REPLACEMENT="HEAD_REF $BRANCH"
if [ -n "${GITHUB_TOKEN:-}" ]; then
    echo "Adding AUTHORIZATION_TOKEN (read from GITHUB_TOKEN at build time)"
    REPLACEMENT="$REPLACEMENT AUTHORIZATION_TOKEN \"\$ENV{GITHUB_TOKEN}\""
fi

# Use different sed syntax for macOS vs Linux
if [[ "$OSTYPE" == "darwin"* ]]; then
    sed -i '' "s|HEAD_REF main|$REPLACEMENT|" "$PORTFILE"
else
    sed -i "s|HEAD_REF main|$REPLACEMENT|" "$PORTFILE"
fi

echo "Updated portfile:"
grep "HEAD_REF" "$PORTFILE"
