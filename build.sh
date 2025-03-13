#!/bin/bash

# Script to build and run the Lunar Mining Simulation

# Default build type
BUILD_TYPE="Debug"
RUN_TESTS=false
CLEAN_BUILD=false
RUN_SIM=false
SIM_ARGS=""

# Parse command line arguments
for arg in "$@"; do
  case $arg in
    -r|--release)
      BUILD_TYPE="Release"
      shift
      ;;
    -t|--test)
      RUN_TESTS=true
      shift
      ;;
    -c|--clean)
      CLEAN_BUILD=true
      shift
      ;;
    -s|--run-sim)
      RUN_SIM=true
      shift
      ;;
    -h|--help)
      echo "Usage: $0 [options] [-- sim_args]"
      echo "Options:"
      echo "  -r, --release    Build in Release mode (default: Debug)"
      echo "  -t, --test       Run tests after building"
      echo "  -c, --clean      Clean build directory before building"
      echo "  -s, --run-sim    Run the simulation after building"
      echo "  -h, --help       Show this help message"
      echo ""
      echo "Additional arguments after -- will be passed to the simulation"
      echo "Example: $0 -r -s -- -t 10 -s 3 -v"
      exit 0
      ;;
    --)
      shift
      SIM_ARGS="$*"
      break
      ;;
    *)
      shift
      ;;
  esac
done

# Create build directory if it doesn't exist
BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ] || [ "$CLEAN_BUILD" = true ]; then
  echo "Creating/cleaning build directory..."
  rm -rf "$BUILD_DIR"
  mkdir -p "$BUILD_DIR"
fi

# Navigate to build directory
cd "$BUILD_DIR" || { echo "Failed to enter build directory"; exit 1; }

# Configure with CMake
echo "Configuring project with CMake (Build type: $BUILD_TYPE)..."
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..

# Build the project
echo "Building project..."
cmake --build . -- -j "$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)"

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
  echo "Running tests..."
  ctest -V
fi

# Run simulation if requested
if [ "$RUN_SIM" = true ]; then
  echo "Running simulation with arguments: $SIM_ARGS"
  ./lunar_mining_sim $SIM_ARGS
fi

echo "Build process completed."