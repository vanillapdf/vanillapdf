#!/bin/bash

# Fix macOS JPEG library version mismatch issues
# This script helps resolve "Wrong JPEG library version" errors on macOS

set -e

echo "VanillaPDF macOS JPEG Library Fix Script"
echo "========================================"

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "This script is designed for macOS only."
    exit 1
fi

# Get architecture
ARCH=$(uname -m)
echo "Detected architecture: $ARCH"

# Check for Homebrew
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Please install Homebrew first:"
    echo "  /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
    exit 1
fi

echo "Step 1: Checking current JPEG installations..."

# Check for existing libjpeg installations
JPEG_LIBS=$(find /usr/local/lib /opt/homebrew/lib -name "*jpeg*" 2>/dev/null || true)
if [[ -n "$JPEG_LIBS" ]]; then
    echo "Found JPEG libraries:"
    echo "$JPEG_LIBS"
else
    echo "No JPEG libraries found in standard locations"
fi

echo ""
echo "Step 2: Installing/updating libjpeg-turbo..."

# Install or update libjpeg-turbo
brew install libjpeg-turbo || brew upgrade libjpeg-turbo

echo ""
echo "Step 3: Cleaning build directory..."

# Clean build directory if it exists
BUILD_DIR="build"
if [[ -d "$BUILD_DIR" ]]; then
    echo "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

echo ""
echo "Step 4: Setting up environment variables..."

# Set environment variables for consistent JPEG library usage
if [[ "$ARCH" == "arm64" ]]; then
    export LDFLAGS="-L/opt/homebrew/lib"
    export CPPFLAGS="-I/opt/homebrew/include"
    export PKG_CONFIG_PATH="/opt/homebrew/lib/pkgconfig"
else
    export LDFLAGS="-L/usr/local/lib"
    export CPPFLAGS="-I/usr/local/include"
    export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig"
fi

echo "Environment configured:"
echo "  LDFLAGS: $LDFLAGS"
echo "  CPPFLAGS: $CPPFLAGS"
echo "  PKG_CONFIG_PATH: $PKG_CONFIG_PATH"

echo ""
echo "Step 5: Configuring CMake with proper JPEG settings..."

# Configure CMake with explicit JPEG library settings
CMAKE_ARGS=""
if [[ "$ARCH" == "arm64" ]]; then
    CMAKE_ARGS="--preset macos-arm64"
else
    CMAKE_ARGS="--preset macos-x64"
fi

# Add JPEG-specific configuration
CMAKE_ARGS="$CMAKE_ARGS -DVANILLAPDF_EXTERNAL_JPEG=OFF"

echo "Running: cmake $CMAKE_ARGS"
cmake $CMAKE_ARGS

echo ""
echo "Step 6: Building the project..."

# Build the project
if [[ "$ARCH" == "arm64" ]]; then
    cmake --build build/macos-arm64
else
    cmake --build build/macos-x64
fi

echo ""
echo "Build completed successfully!"
echo ""
echo "To test the fix, run the DCTDecodeFilter test:"
if [[ "$ARCH" == "arm64" ]]; then
    echo "  cd build/macos-arm64 && ctest -R DCTDecodeFilter"
else
    echo "  cd build/macos-x64 && ctest -R DCTDecodeFilter"
fi

echo ""
echo "If you still encounter issues, try:"
echo "1. Restart your terminal session"
echo "2. Run 'brew doctor' to check for Homebrew issues"
echo "3. Use the VANILLAPDF_EXTERNAL_JPEG=ON option to use system JPEG"