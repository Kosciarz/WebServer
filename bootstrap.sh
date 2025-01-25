#!/bin/bash

echo Initializing submodules if needed
git submodule update --init --recursive

# Navigate to vcpkg
cd external/vcpkg

echo Bootstraping vcpkg
./bootstrap-vcpkg.sh

echo Installing dependencies
./vcpkg install boost-asio boost-system

# Navigate back to root
cd ../..