#include <cerrno>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include "Truck.hpp"

int main(int argc, char* argv[]) {
    // Default configuration
    int numTrucks = 5;
    double simulationDuration = 72.0; // 72 hours

    Truck truck(1);

    std::cout << truck.getId() << std::endl;

    // Allow command-line configuration
    if (argc >= 2) {
        numTrucks = std::stoi(argv[2]);
    }
    
    // Print configuration
    std::cout << "=== LUNAR HELIUM-3 MINING SIMULATION (TRUCKS ONLY) ===" << std::endl;
    std::cout << "Number of Mining Trucks: " << numTrucks << std::endl;
    std::cout << "Simulation Duration: " << simulationDuration << " hours" << std::endl;
    std::cout << std::endl;

    // Run the simulation
    // simulateMining(numTrucks, simulationDuration);
    
    return 0;
}