#include <gtest/gtest.h>
#include "../src/TruckSim.hpp"

class SimTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up a fresh truck for each test
        sim11 = new TruckSim(1, 1);
    }

    void TearDown() override {
        delete sim11;
    }

    TruckSim* sim11;
};

// Test Sim initialization
TEST_F(SimTest, Initialization) {
    EXPECT_EQ(sim11->getNumTrucks(), 1);
    EXPECT_EQ(sim11->getNumStations(), 1);
}