#include <gtest/gtest.h>
#include "../src/TruckSim.hpp"
#include <sstream>
#include <iostream>

class SimTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up simulations with different configurations
        sim11 = new TruckSim(1, 1);
        sim32 = new TruckSim(3, 2);
        sim54 = new TruckSim(5, 4);
    }

    void TearDown() override {
        delete sim11;
        delete sim32;
        delete sim54;
    }

    TruckSim* sim11; // 1 truck, 1 station
    TruckSim* sim32; // 3 trucks, 2 stations
    TruckSim* sim54; // 5 trucks, 4 stations
};

// Test Sim initialization
TEST_F(SimTest, Initialization) {
    EXPECT_EQ(sim11->getNumTrucks(), 1);
    EXPECT_EQ(sim11->getNumStations(), 1);
    
    EXPECT_EQ(sim32->getNumTrucks(), 3);
    EXPECT_EQ(sim32->getNumStations(), 2);
    
    EXPECT_EQ(sim54->getNumTrucks(), 5);
    EXPECT_EQ(sim54->getNumStations(), 4);
}

// Test simulation with one truck and one station
TEST_F(SimTest, SingleTruckSingleStation) {
    // Redirect stdout to capture output
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    
    // Run the simulation
    sim11->simulate();
    
    // Restore cout
    std::cout.rdbuf(oldCout);
    
    // Check output contains expected sections
    std::string output = buffer.str();
    
    // With one truck and one station, there should be no idle time
    // NOTE: I would typically not parse the std out for this and would expose 
    // more variables out to the truck sim class
    EXPECT_TRUE(output.find("Idle Time Total: 0s") != std::string::npos);
}

// Test simulation with more stations than trucks
TEST_F(SimTest, MoreStationsThanTrucks) {
    // Redirect stdout to capture output
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    
    // Run the simulation
    sim54->simulate();
    
    // Restore cout
    std::cout.rdbuf(oldCout);
    
    // Check output contains expected sections
    std::string output = buffer.str();
    
    // Should have stats for 5 trucks
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(output.find("Truck ID " + std::to_string(i)) != std::string::npos);
    }
    
    // Should have stats for 4 stations
    for (int i = 0; i < 4; i++) {
        EXPECT_TRUE(output.find("Station " + std::to_string(i) + " stats:") != std::string::npos);
    }
    
}