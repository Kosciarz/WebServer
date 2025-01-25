#!/bin/bash

# Initialize submodule if needed
git submodule update --init --recursive

# Navigate to vcpkg
cd external/vcpkg

# Bootstrap vcpkg
./bootstrap-vcpkg.sh

# Install dependencies
./vcpkg install boost-asio boost-system