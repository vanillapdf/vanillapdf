#!/bin/bash
# Test script to verify the CMake export verification target works

set -e

echo "Testing CMake export verification target..."

# Create a temporary build directory
TEMP_DIR=$(mktemp -d)
echo "Using temporary directory: $TEMP_DIR"

cd "$TEMP_DIR"

# Configure with shared libraries enabled
echo "Configuring with shared libraries..."
cmake /workspace -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release

# Build the library
echo "Building library..."
cmake --build . --target vanillapdf

# Test the verify_exports target
echo "Testing verify_exports target..."
if cmake --build . --target verify_exports; then
    echo "✅ Export verification target works!"
    exit_code=0
else
    echo "❌ Export verification target failed!"
    exit_code=1
fi

# Cleanup
cd /
rm -rf "$TEMP_DIR"

exit $exit_code