#!/bin/bash

clang-format -i -style=file src/*.cpp src/*.hpp build.sh CMakeLists.txt

echo "Code formatting completed."