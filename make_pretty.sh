#!/bin/bash

clang-format -i -style=file src/*.cpp src/*.hpp

echo "Code formatting completed."