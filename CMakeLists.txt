cmake_minimum_required(VERSION 3.10)
project(LunarMiningSim VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compile flags (optimize for release builds)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
endif()

# Add the main final executable
add_executable(lunar_mining_sim src/main.cpp)

# Create an install target
install(TARGETS lunar_mining_sim
        RUNTIME DESTINATION bin)

# Enable testing
enable_testing()

# Add a basic test
add_test(NAME BasicSimulationTest 
         COMMAND lunar_mining_sim -t 3 -s 2 -d 10)

# Add a test to run parameter analysis
add_test(NAME ParameterAnalysisTest
         COMMAND lunar_mining_sim --analyze)