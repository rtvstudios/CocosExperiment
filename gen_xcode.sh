#!/bin/bash

rm -rf build/xcode
mkdir -p build/xcode
cd build/xcode
cmake -G "Xcode"  ../..

