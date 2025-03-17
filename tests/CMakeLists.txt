set(TEST_SOURCES
    TruckTests.cpp
    StationTests.cpp
    SimTests.cpp
)

# Add test executable
add_executable(unit_tests ${TEST_SOURCES})

# Link with Google Test and our testable library
target_link_libraries(unit_tests 
    gtest 
    gtest_main
)

# Discover and register tests
gtest_discover_tests(unit_tests)