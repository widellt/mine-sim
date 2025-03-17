#include <gtest/gtest.h>
#include "../src/Truck.hpp"
#include "../src/Station.hpp"

class StationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up a fresh station for each test
        station = new Station(1, 1);
        
        // Set up some trucks for queue testing
        truck1 = new Truck(1, 1);
        truck2 = new Truck(2, 1);
        truck3 = new Truck(3, 1);
    }

    void TearDown() override {
        delete station;
        delete truck1;
        delete truck2;
        delete truck3;
    }

    Station* station;
    Truck* truck1;
    Truck* truck2;
    Truck* truck3;
};

// Test station initialization
TEST_F(StationTest, Initialization) {
    EXPECT_EQ(station->getId(), 1);
}