#include "ThreadPool.hpp"
#include "Station.hpp"
#include "Truck.hpp"
#include "TruckSim.hpp"
#include <cerrno>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <chrono>
#include "Log.hpp"

int main(int argc, char *argv[]) {
  // Default configuration
  int numTrucks = 4;
  int numStations = 2;
  int numThreads = 0; // 0 means use hardware_concurrency
  bool verbose = false;
  bool benchmarkMode = false;

  // Allow command-line configuration
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <options>" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -t <num>     Number of trucks (default: 4)" << std::endl;
    std::cerr << "  -s <num>     Number of stations (default: 2)" << std::endl;
    std::cerr << "  -p <num>     Number of threads in the thread pool (default: hardware concurrency)" << std::endl;
    std::cerr << "  -v           Verbose mode" << std::endl;
    std::cerr << "  -b           Benchmark mode (run with different thread counts)" << std::endl;
    return 1;
  }

  for (auto i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-t" && i + 1 < argc) {
      numTrucks = std::stoi(argv[++i]);
    }
    else if (arg == "-s" && i + 1 < argc) {
      numStations = std::stoi(argv[++i]);
    }
    else if (arg == "-p" && i + 1 < argc) {
      numThreads = std::stoi(argv[++i]);
    }
    else if (arg == "-v") {
      verbose = true;
    }
    else if (arg == "-b") {
      benchmarkMode = true;
    }
  }

  // Print configuration
  std::cout << "=== LUNAR HELIUM-3 MINING SIMULATION ===" << std::endl;
  std::cout << "Number of Mining Trucks: " << numTrucks << std::endl;
  std::cout << "Number of Unload Stations: " << numStations << std::endl;
  
  if (benchmarkMode) {
    // Run the simulation with different thread counts to benchmark
    std::cout << "\n=== STARTING BENCHMARK MODE ===" << std::endl;
    std::cout << "Running simulation with varying thread counts..." << std::endl;
    
    // Thread counts to test (1, 2, 4, hardware_concurrency)
    std::vector<int> threadCounts = {1, 2, 4, (int)std::thread::hardware_concurrency()};
    
    for (int threads : threadCounts) {
      std::cout << "\nRunning with " << threads << " threads:" << std::endl;
      
      // Measure execution time
      auto startTime = std::chrono::high_resolution_clock::now();
      
      // Run the simulation
      TruckSim sim(numTrucks, numStations, threads);
      sim.simulate();
      
      auto endTime = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
          endTime - startTime).count();
      
      std::cout << "Execution time: " << duration << " ms" << std::endl;
    }
    
    std::cout << "\n=== BENCHMARK COMPLETE ===" << std::endl;
  } 
  else {
    // Standard simulation run
    if (numThreads > 0) {
      std::cout << "Thread Pool Size: " << numThreads << std::endl;
    } else {
      std::cout << "Thread Pool Size: auto (" << std::thread::hardware_concurrency() << ")" << std::endl;
    }
    std::cout << std::endl;
    
    // Measure execution time
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Run the simulation
    TruckSim sim(numTrucks, numStations, numThreads);
    sim.simulate();
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime).count();
    
    std::cout << "\nExecution time: " << duration << " ms" << std::endl;
  }

  return 0;
}