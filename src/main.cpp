#include <cerrno>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include "Truck.hpp"
#include "Station.hpp"
#include "TruckSim.hpp"

int main(int argc, char* argv[]) {
    // Default configuration
    int numTrucks = 4;
    int numStations = 2;

    // Allow command-line configuration
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <numTrucks> <simulationDuration>" << std::endl;
        return 1;
    }

    for (auto i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-t") {
            numTrucks = std::stoi(argv[i+1]);
        }
        if (std::string(argv[i]) == "-s") {
            numStations = std::stoi(argv[i+1]);
        }
    }

    // Print configuration
    std::cout << "=== LUNAR HELIUM-3 MINING SIMULATION ===" << std::endl;
    std::cout << "Number of Mining Trucks: " << numTrucks << std::endl;
    std::cout << "Number of Unload Stations: " << numStations << std::endl;
    std::cout << std::endl;

    // Run the simulation
    TruckSim sim(numTrucks, numStations);
    sim.simulate();
    
    return 0;
}