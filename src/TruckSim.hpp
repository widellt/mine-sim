#pragma once
// Simulation class with ThreadPool integration
#include "Log.hpp"
#include "Station.hpp"
#include "ThreadPool.hpp"
#include "Truck.hpp"
#include <iostream>
#include <queue>
#include <vector>
#include <future>
#include <iomanip>

class TruckSim {
private:
  // Trucks and stations
  std::vector<Truck> m_trucks;
  std::vector<Station> m_unloadStations;
  std::queue<Truck*> m_waitingTrucks;
  uint32_t m_numTrucks;
  uint32_t m_numStations;
  double m_currTime = 0;

  // Efficiency metrics
  float m_totalMiningTime = 0.0f;
  float m_totalUnloadTime = 0.0f;
  float m_totalTravelTime = 0.0f;
  float m_totalIdleTime = 0.0f;
  float m_totalOperationalTime = 0.0f;
  float m_totalStationOccupiedTime = 0.0f;
  float m_totalStationIdleTime = 0.0f;
  
  // Time step variables
  static inline constexpr float dt = 1.0f; // s
  static inline constexpr float SIMULATION_DURATION = 259200.0f / dt;
  
  // Thread pool
  ThreadPool m_threadPool;

public:
  TruckSim(int numTrucks, int numStations, int numThreads = 0)
      : m_numTrucks(numTrucks), m_numStations(numStations),
        m_threadPool(numThreads > 0 ? numThreads : std::thread::hardware_concurrency()) {
    // Pre-allocate vectors to avoid resizing
    m_trucks.reserve(numTrucks);
    m_unloadStations.reserve(numStations);
    
    // Initialize trucks and unload stations
    for (int i = 0; i < numTrucks; i++) {
      m_trucks.emplace_back(i, dt);
    }
    for (int i = 0; i < numStations; i++) {
      m_unloadStations.emplace_back(i, dt);
    }
    
    Logger::LOGI("Initialized simulation with " + std::to_string(numTrucks) + 
                " trucks, " + std::to_string(numStations) + " stations, and " + 
                std::to_string(m_threadPool.size()) + " worker threads");
  }

  uint32_t getNumTrucks() const { return m_numTrucks; }
  uint32_t getNumStations() const { return m_numStations; }

  void simulate() {
    Logger::LOGI("Starting simulation...");

    // Continue in while loop until we reach end time in increments of dt
    while (m_currTime < SIMULATION_DURATION) {
      m_currTime += dt;
      
      // Update stations (not parallelized due to potential race conditions)
      for (auto &station : m_unloadStations) {
        station.update();
      }

      // Process trucks in parallel using the thread pool
      std::vector<std::future<void>> updateFutures;
      updateFutures.reserve(m_trucks.size());
      
      // Submit truck updates to thread pool
      for (auto &truck : m_trucks) {
        updateFutures.push_back(
          m_threadPool.enqueue([&truck]() {
            truck.update();
          })
        );
      }
      
      // Wait for all truck updates to complete
      for (auto &future : updateFutures) {
        future.wait();
      }

      // Maintain truck-station assignments (not parallelized due to shared resource access)
      assignTrucksToStations();
    }
    
    calculateEfficiencyMetrics();
    printTruckStats();
    printStationStats();
    printEfficiencyStats();
    
    Logger::LOGI("Simulation completed");
  }

private:
  void assignTrucksToStations() {
    // Collect all trucks that need a station
    std::vector<Truck*> trucksNeedingStation;
    
    for (auto &truck : m_trucks) {
      if ((truck.getState() == TruckState::UNLOADING || 
           truck.getState() == TruckState::IDLE) && 
          !truck.hasStation()) {
        trucksNeedingStation.push_back(&truck);
      }
    }
    
    // Find available stations
    std::vector<Station*> availableStations;
    
    for (auto &station : m_unloadStations) {
      if (station.getTruckInStation() == nullptr) {
        availableStations.push_back(&station);
      }
    }
    
    // Match trucks to stations efficiently
    size_t numAssignments = std::min(trucksNeedingStation.size(), availableStations.size());
    
    for (size_t i = 0; i < numAssignments; ++i) {
      Truck* truck = trucksNeedingStation[i];
      Station* station = availableStations[i];
      
      // Update truck state if it was waiting
      if (truck->getState() == TruckState::IDLE) {
        truck->setState(TruckState::UNLOADING);
      }
      
      // Assign truck to station
      truck->setHasStation(true);
      station->setTruckInStation(truck);
    }
    
    // Set remaining trucks to IDLE if they need a station but none available
    for (size_t i = numAssignments; i < trucksNeedingStation.size(); ++i) {
      trucksNeedingStation[i]->setState(TruckState::IDLE);
    }
  }

  void calculateEfficiencyMetrics() {
    // Initialize totals
    m_totalMiningTime = 0.0f;
    m_totalUnloadTime = 0.0f;
    m_totalTravelTime = 0.0f;
    m_totalIdleTime = 0.0f;
    
    // Sum all time components from trucks
    for (auto &truck : m_trucks) {
      m_totalMiningTime += truck.getMiningTimeTotal() * dt;
      m_totalUnloadTime += truck.getUnloadTimeTotal() * dt;
      m_totalTravelTime += truck.getTravelTimeTotal() * dt;
      m_totalIdleTime += truck.getIdleTimeTotal() * dt;
    }
    
    // Calculate station efficiency
    m_totalStationOccupiedTime = 0.0f;
    for (auto &station : m_unloadStations) {
      m_totalStationOccupiedTime += station.getTimeOccupied();
    }
    
    // Total possible station operational time
    float totalPossibleStationTime = SIMULATION_DURATION * dt * m_numStations;
    m_totalStationIdleTime = totalPossibleStationTime - m_totalStationOccupiedTime;
    
    // Calculate total operational time across all trucks
    m_totalOperationalTime = m_totalMiningTime + m_totalUnloadTime + m_totalTravelTime + m_totalIdleTime;
  }

  void printTruckStats() {
    std::cout << "=== TRUCK STATISTICS ===" << std::endl;
    for (auto &truck : m_trucks) {
      std::cout << "Truck ID " << truck.getId() << std::endl;

      // Multiply the times by dt to report values in seconds
      std::cout << "Mining Time Total: " << truck.getMiningTimeTotal() * dt
                << "s" << std::endl;
      std::cout << "Unload Time Total: " << truck.getUnloadTimeTotal() * dt
                << "s" << std::endl;
      std::cout << "Travel Time Total: " << truck.getTravelTimeTotal() * dt
                << "s" << std::endl;
      std::cout << "Idle Time Total: " << truck.getIdleTimeTotal() * dt << "s"
                << std::endl;

      std::cout << "Total Time: "
                << (truck.getMiningTimeTotal() * dt) +
                       (truck.getUnloadTimeTotal() * dt) +
                       (truck.getTravelTimeTotal() * dt) +
                       (truck.getIdleTimeTotal() * dt)
                << "s" << std::endl;
      std::cout << std::endl;
    }
  }

  void printStationStats() {
    std::cout << "=== STATION STATISTICS ===" << std::endl;
    for (auto &station : m_unloadStations) {
      std::cout << "Station " << station.getId() << " stats:" << std::endl;
      std::cout << "Time occupied: " << station.getTimeOccupied() << "s" << std::endl;

      std::unordered_map<uint32_t, float> truckTimes = station.getTruckTimes();

      for (auto &truck : truckTimes) {
        std::cout << "Truck ID: " << truck.first << " time: " << truck.second
                  << "s" << std::endl;
      }
      std::cout << std::endl;
    }
  }
  
  void printEfficiencyStats() {
    std::cout << "=== EFFICIENCY STATISTICS ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    // Print overall time breakdown
    float simDurationSecs = SIMULATION_DURATION * dt;
    std::cout << "Simulation Duration: " << simDurationSecs << "s (" 
              << simDurationSecs / 3600.0f << " hours)" << std::endl;
    std::cout << std::endl;
    
    // Fleet statistics
    std::cout << "Fleet Statistics:" << std::endl;
    std::cout << "Total Mining Time: " << m_totalMiningTime << "s (" 
              << (m_totalMiningTime / m_totalOperationalTime) * 100.0f << "%)" << std::endl;
    std::cout << "Total Travel Time: " << m_totalTravelTime << "s (" 
              << (m_totalTravelTime / m_totalOperationalTime) * 100.0f << "%)" << std::endl;
    std::cout << "Total Unload Time: " << m_totalUnloadTime << "s (" 
              << (m_totalUnloadTime / m_totalOperationalTime) * 100.0f << "%)" << std::endl;
    std::cout << "Total Idle Time: " << m_totalIdleTime << "s (" 
              << (m_totalIdleTime / m_totalOperationalTime) * 100.0f << "%)" << std::endl;
    std::cout << std::endl;
    
    // Average per truck
    std::cout << "Average Per Truck:" << std::endl;
    std::cout << "Avg Mining Time: " << m_totalMiningTime / m_numTrucks << "s" << std::endl;
    std::cout << "Avg Travel Time: " << m_totalTravelTime / m_numTrucks << "s" << std::endl;
    std::cout << "Avg Unload Time: " << m_totalUnloadTime / m_numTrucks << "s" << std::endl;
    std::cout << "Avg Idle Time: " << m_totalIdleTime / m_numTrucks << "s" << std::endl;
    std::cout << std::endl;
    
    // Station Utilization
    float stationUtilization = (m_totalStationOccupiedTime / (simDurationSecs * m_numStations)) * 100.0f;
    std::cout << "Station Utilization:" << std::endl;
    std::cout << "Total Station Occupied Time: " << m_totalStationOccupiedTime << "s" << std::endl;
    std::cout << "Total Station Idle Time: " << m_totalStationIdleTime << "s" << std::endl;
    std::cout << "Station Utilization Rate: " << stationUtilization << "%" << std::endl;
    std::cout << std::endl;
    
    // Overall efficiency metrics
    float truckIdleRate = (m_totalIdleTime / m_totalOperationalTime) * 100.0f;
    float truckProductiveRate = 100.0f - truckIdleRate;
    
    std::cout << "Overall Efficiency Metrics:" << std::endl;
    std::cout << "Truck Productive Time Rate: " << truckProductiveRate << "%" << std::endl;
    std::cout << "Truck Idle Time Rate: " << truckIdleRate << "%" << std::endl;
    std::cout << "Station Utilization Rate: " << stationUtilization << "%" << std::endl;
    
    // Calculate system bottleneck
    std::cout << std::endl << "System Analysis:" << std::endl;
    if (truckIdleRate > (100.0f - stationUtilization)) {
      std::cout << "BOTTLENECK: Insufficient unloading stations - trucks waiting too long" << std::endl;
      std::cout << "RECOMMENDATION: Add more unloading stations" << std::endl;
    } else if (stationUtilization < 70.0f) {
      std::cout << "BOTTLENECK: Too many unloading stations - stations underutilized" << std::endl;
      std::cout << "RECOMMENDATION: Reduce number of stations or add more trucks" << std::endl;
    } else {
      std::cout << "System appears well-balanced" << std::endl;
    }
    
    // Calculate optimal ratio
    float currentRatio = static_cast<float>(m_numTrucks) / static_cast<float>(m_numStations);
    float optimalRatio = 0.0f;
    
    if (truckIdleRate > 10.0f && stationUtilization > 90.0f) {
      // Need more stations
      optimalRatio = currentRatio * 0.8f;
    } else if (stationUtilization < 70.0f && truckIdleRate < 5.0f) {
      // Need more trucks or fewer stations
      optimalRatio = currentRatio * 1.2f;
    } else {
      optimalRatio = currentRatio;
    }
    
    std::cout << "Current Truck:Station Ratio: " << currentRatio << std::endl;
    std::cout << "Estimated Optimal Ratio: " << optimalRatio << std::endl;
  }
};