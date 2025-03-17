#include <gtest/gtest.h>
#include "../src/Truck.hpp"
#include "../src/Station.hpp"

class StationTest : public ::testing::Test {
protected:

    float dt = 1.0f;
    void SetUp() override {
        // Set up a fresh station for each test
        station = new Station(1, dt);
    }
    void TearDown() override {
        delete station;
    }
    Station* station;
};

// Test station initialization
TEST_F(StationTest, Initialization) {
    EXPECT_EQ(station->getId(), 1);
}