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
    EXPECT_FALSE(truck->hasStation());
    EXPECT_GT(truck->getMiningTimeLeft(), 0.0f);
}

// Test state transition from Mining to Traveling_To_Station
TEST_F(TruckTest, MiningToTravelingTransition) {
    // Force mining time to be almost finished
    float initialMiningTime = truck->getMiningTimeLeft();
    // Total time should be 0 at sim start
    EXPECT_EQ(truck->getMiningTimeTotal(), 0);
    float lastMiningTime = initialMiningTime;
    
    // Simulate time passing until mining is done
    while (true) {
        lastMiningTime = truck->getMiningTimeLeft();
        truck->update();
        // Truck will reset its mining time when it finishes, break if last time > new
        if(truck->getMiningTimeLeft() > lastMiningTime) {
            break;
        }
    }
    
    // Should transition to traveling to station
    EXPECT_EQ(truck->getState(), TruckState::TRAVELING_TO_STATION);
    // Total time should equate to initial time
    EXPECT_EQ(truck->getMiningTimeTotal(), initialMiningTime);
}

// Rest of state transition tests would go here...