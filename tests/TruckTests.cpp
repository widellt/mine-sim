#include <gtest/gtest.h>
#include "../src/Truck.hpp"

class TruckTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up a fresh truck for each test
        truck = new Truck(1, 1);
    }

    void TearDown() override {
        delete truck;
    }

    Truck* truck;
};

// Test truck initialization
TEST_F(TruckTest, Initialization) {
    EXPECT_EQ(truck->getId(), 1);
    EXPECT_EQ(truck->getState(), TruckState::MINING);
    
}