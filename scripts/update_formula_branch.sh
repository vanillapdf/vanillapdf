#!/bin/bash
# Update Homebrew formula to use a specific branch for HEAD
# Usage: ./update_formula_branch.sh <branch_name>

set -e

BRANCH="${1:-main}"
FORMULA="homebrew/vanillapdf.rb"

if [ ! -f "$FORMULA" ]; then
    echo "Error: $FORMULA not found"
    exit 1
fi

echo "Updating HEAD branch to: $BRANCH"

# Use different sed syntax for macOS vs Linux
if [[ "$OSTYPE" == "darwin"* ]]; then
    sed -i '' "s|branch: \"main\"|branch: \"$BRANCH\"|" "$FORMULA"
else
    sed -i "s|branch: \"main\"|branch: \"$BRANCH\"|" "$FORMULA"
fi

echo "Updated formula:"
grep "head" "$FORMULA"
