#!/bin/bash
set -e
VERSION="${VERSION:-dev}"
BUILD_DIR="/project/build"
RELEASE_DIR="/project/releases/${VERSION}"
mkdir -p "$BUILD_DIR" "$RELEASE_DIR"
cd /project
cmake -S . -B "$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" --config Release
# copy resulting binary
if [ -f "$BUILD_DIR/mixcons.exe" ]; then
  cp "$BUILD_DIR/mixcons.exe" "$RELEASE_DIR/"
elif [ -f "$BUILD_DIR/mixcons" ]; then
  cp "$BUILD_DIR/mixcons" "$RELEASE_DIR/"
fi
