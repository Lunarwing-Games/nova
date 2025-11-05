#!/usr/bin/env bash

set -euo pipefail

LOCAL_TEMP="$PWD/tmp"
VENDOR_DIR="$LOCAL_TEMP/vendor"
IMGUI_DIR="$VENDOR_DIR/imgui"
STB_DIR="$VENDOR_DIR/stb"
CGLM_DIR="$VENDOR_DIR/cglm"
GLFW_DIR="$VENDOR_DIR/glfw"
GLEW_DIR="$VENDOR_DIR/glew"
OPENAL_DIR="$VENDOR_DIR/openal"

mkdir -pv "$LOCAL_TEMP" "$VENDOR_DIR" \
          "$IMGUI_DIR" "$STB_DIR" "$CGLM_DIR" \
          "$GLFW_DIR" "$GLEW_DIR" "$OPENAL_DIR"

download_and_extract() {
    local url="$1"
    local outdir="$2"
    local tmpfile
    tmpfile="$(mktemp "$LOCAL_TEMP/XXXXXX.tar.gz")"
    echo "? Downloading $(basename "$url")..."
    wget -q -O "$tmpfile" "$url"
    echo "* Extracting to $outdir..."
    tar -xzf "$tmpfile" -C "$outdir" --strip-components=1
    rm -f "$tmpfile"
}

# ImGUI
download_and_extract "https://github.com/ocornut/imgui/archive/refs/tags/v1.92.4.tar.gz" "$IMGUI_DIR"

# stb files (header-only, so just pull the raw files from the master branch)
echo "📦 Fetching stb headers..."
wget -q -O "$STB_DIR/stb_image.h" https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
wget -q -O "$STB_DIR/stb_image_write.h" https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
wget -q -O "$STB_DIR/stb_rect_pack.h" https://raw.githubusercontent.com/nothings/stb/master/stb_rect_pack.h

# CGLM
download_and_extract "https://github.com/recp/cglm/archive/refs/tags/v0.9.4.tar.gz" "$CGLM_DIR"

# GLFW
download_and_extract "https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz" "$GLFW_DIR"

# GLEW
download_and_extract "https://github.com/nigels-com/glew/archive/refs/tags/glew-2.2.0.tar.gz" "$GLEW_DIR"

# OpenAL
download_and_extract "https://github.com/kcat/openal-soft/archive/refs/tags/1.23.1.tar.gz" "$OPENAL_DIR"

ACTUAL_VENDOR="$PWD/vendor"
ACTUAL_IMGUI_DIR="$ACTUAL_VENDOR/imgui"
ACTUAL_STB_DIR="$ACTUAL_VENDOR/stb"
ACTUAL_CGLM_DIR="$VACTUAL_VENDOR/cglm"
ACTUAL_GLFW_DIR="$ACTUAL_VENDOR/glfw"
ACTUAL_GLEW_DIR="$ACTUAL_VENDOR/glew"
ACTUAL_OPENAL_DIR="$ACTUAL_VENDOR/openal"

# Copy step 1, stb (header-only)
cp -r "$STB_DIR" "$ACTUAL_STB_DIR"

# Copy step 2, imgui (also kinda header-only)

# Build and copy over GLFW (step 3)

# Build and copy over GLEW (step 4)

# Copy cglm (step 5)

# Remember, Steamworks has to be vendored by *you!*
# This script cannot do that, since it goes against the
# SDK Access Agreement
# 
