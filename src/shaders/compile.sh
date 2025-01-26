#!/bin/bash

# Ensure glslc is installed
if ! command -v glslc &> /dev/null
then
    echo "glslc could not be found. Please install Vulkan SDK."
    exit 1
fi

# Directory containing shader files
SHADER_DIR=$(pwd)

# Output directory for compiled SPIR-V files
OUTPUT_DIR="${SHADER_DIR}/compiled"
mkdir -p "$OUTPUT_DIR"

# Compile all .vert and .frag files
for SHADER_FILE in "${SHADER_DIR}"/*.{vert,frag}; do
    if [ -f "$SHADER_FILE" ]; then
        FILENAME=$(basename -- "$SHADER_FILE")
        EXT="${FILENAME##*.}"
        BASENAME="${FILENAME%.*}"
        OUTPUT_FILE="${OUTPUT_DIR}/${BASENAME}.${EXT}.spv"

        echo "Compiling $FILENAME -> $OUTPUT_FILE"
        glslc "$SHADER_FILE" -o "$OUTPUT_FILE"

        if [ $? -eq 0 ]; then
            echo "$FILENAME compiled successfully!"
        else
            echo "Error compiling $FILENAME"
        fi
    fi
done

echo "All shaders compiled. Compiled files are in: $OUTPUT_DIR"
