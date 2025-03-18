#include <gtest/gtest.h>
#include "../src/Truck.hpp"
#include "../src/Station.hpp"

class StationTest : public ::testing::Test {
protected:

    float dt = 1.0f;
    void SetUp() override {
        // Set up a fresh station for each test
        station = new Station(1, dt);
        truck = new Truck(2, dt);
    }
    void TearDown() override {
        delete station;
        delete truck;
    }
    Station* station;
    Truck* truck;
};

// Test station initialization
TEST_F(StationTest, Initialization) {
    EXPECT_EQ(station->getId(), 1);
}

// Test station update with a truck
TEST_F(StationTest, StationUpdate) {
    // Prepare a truck in unloading state
    truck->setState(TruckState::UNLOADING);
    
    // Assign truck to station
    station->setTruckInStation(truck);
    
    // Keep initial reference to ensure truck doesn't change
    Truck* initialTruck = station->getTruckInStation();
    
    // Run one update cycle
    station->update();
    
    // Verify time accounting
    EXPECT_GT(station->getTimeOccupied(), 0.0f);
    EXPECT_EQ(station->getTimeOccupied(), dt);
    
    // Verify truck ID is tracked in the truck times map
    auto truckTimes = station->getTruckTimes();
    EXPECT_EQ(truckTimes.size(), 1);
    EXPECT_EQ(truckTimes[truck->getId()], dt);
    
    // Truck should still be in station
    EXPECT_EQ(station->getTruckInStation(), initialTruck);
}

// Test station releasing a truck after unloading
TEST_F(StationTest, TruckReleasing) {
    // Set up truck to be almost done unloading
    truck->setState(TruckState::UNLOADING);
    // Force the unload time to be minimal
    
    // Assign truck to station
    station->setTruckInStation(truck);
    
    // Get initial occupied time
    float initialTimeOccupied = station->getTimeOccupied();
    
    // Run update until truck is released
    while (station->getTruckInStation() != nullptr) {
        station->update();
        truck->update();
    }
    
    // Verify the truck is released
    EXPECT_EQ(station->getTruckInStation(), nullptr);
    
    // Time occupied should have increased
    EXPECT_GT(station->getTimeOccupied(), initialTimeOccupied);
}