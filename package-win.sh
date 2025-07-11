#!/bin/bash
set -e

DIST_DIR="dist-win-x64"
EXE="build-win-x64/walltaker_universal.exe"
DLL_DIR="deps/libcurl-windows/bin"

# Create the output directory
mkdir -p "$DIST_DIR"

# Copy the executable
cp "$EXE" "$DIST_DIR/"

# Copy all DLLs from the DLL directory
find "$DLL_DIR" -maxdepth 1 -type f -name '*.dll' -exec cp {} "$DIST_DIR/" \;

# Copy common mingw-w64 runtime DLLs if found
for DLL in libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll; do
  DLL_PATH=$(find /usr/x86_64-w64-mingw32/ -name "$DLL" 2>/dev/null | head -n 1)
  if [ -n "$DLL_PATH" ]; then
    cp "$DLL_PATH" "$DIST_DIR/"
  fi
  # Also check in /usr/lib/gcc/x86_64-w64-mingw32/*
  DLL_PATH2=$(find /usr/lib/gcc/x86_64-w64-mingw32/ -name "$DLL" 2>/dev/null | head -n 1)
  if [ -n "$DLL_PATH2" ]; then
    cp "$DLL_PATH2" "$DIST_DIR/"
  fi
  # Also check in /usr/x86_64-w64-mingw32/lib/
  DLL_PATH3=$(find /usr/x86_64-w64-mingw32/lib/ -name "$DLL" 2>/dev/null | head -n 1)
  if [ -n "$DLL_PATH3" ]; then
    cp "$DLL_PATH3" "$DIST_DIR/"
  fi
  # Also check in /usr/x86_64-w64-mingw32/bin/
  DLL_PATH4=$(find /usr/x86_64-w64-mingw32/bin/ -name "$DLL" 2>/dev/null | head -n 1)
  if [ -n "$DLL_PATH4" ]; then
    cp "$DLL_PATH4" "$DIST_DIR/"
  fi

done

# Optionally, copy README or license files
cp deps/libcurl-windows/README.txt "$DIST_DIR/" || true

echo "Packaging complete. Files are in $DIST_DIR/"
