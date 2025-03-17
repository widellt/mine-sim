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

# Option to build tests
option(BUILD_TESTS "Build the unit tests" ON)

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

# Setup GTest
if(BUILD_TESTS)
    # Fetch GTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Enable testing with CTest
    include(GoogleTest)

    # Create the test directory
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    
    # Add the test subdirectory
    add_subdirectory(tests)
endif()