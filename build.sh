#!/bin/bash
set -e

# Build for Linux x64
echo "Building for Linux x64..."
cmake -B build-linux-x64 -DCMAKE_BUILD_TYPE=Release -Dnlohmann_json_DIR=$(pwd)/deps/nlohmann_json
cmake --build build-linux-x64 --config Release

# Build for Windows x64 (cross-compile)
echo "Building for Windows x64..."
cmake -B build-win-x64 \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-win-x64 --config Release

echo "Builds complete. Linux x64 binaries are in build-linux-x64/, Windows x64 binaries are in build-win-x64/"
