#!/bin/bash

# Create 3rdParty directory if it doesn't exist
mkdir -p 3rdParty
cd 3rdParty

# GLFW: Download, compile, and install
echo "Downloading and installing GLFW..."
git clone https://github.com/glfw/glfw.git
cd glfw

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../../GLFW
make -j12
make install
cd ../..

# Remove cloned GLFW repository
rm -rf glfw
echo "GLFW installed and source removed."

# GLM: Download and install
echo "Downloading and installing GLM..."
git clone https://github.com/g-truc/glm.git
cd glm

# GLM is header-only, so copy headers to the 3rdParty directory
mkdir -p ../GLM
cp -r glm ../GLM
cd ..

# Remove cloned GLM repository
rm -rf glm
echo "GLM installed and source removed."

echo "Installation complete."
