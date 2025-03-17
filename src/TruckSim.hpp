// Simplified simulation class for trucks only
#include "Log.hpp"
#include "Station.hpp"
#include "Truck.hpp"
#include <iostream>
#include <queue>
#include <vector>

class TruckSim {
private:
  // Let's assume one truck and one unload station to begin with
  std::vector<Truck> m_trucks;
  std::vector<Station> m_unloadStations;
  std::queue<Truck> m_waitingLine;
  uint32_t m_numTrucks;
  uint32_t m_numStations;
  double m_currTime = 0;
  static constexpr float dt = 1.0f; // s
  static constexpr float SIMULATION_DURATION = 259200.0f / dt;

public:
  TruckSim(int numTrucks, int numStations)
      : m_numTrucks(numTrucks), m_numStations(numStations) {
    // Initialize trucks and unload stations
    for (int i = 0; i < numTrucks; i++) {
      m_trucks.push_back(Truck(i, dt));
    }
    for (int i = 0; i < numStations; i++) {
      m_unloadStations.push_back(Station(i, dt));
    }
  }

  uint32_t getNumTrucks() { return m_numTrucks; }
  uint32_t getNumStations() { return m_numStations; }

  void simulate() {

    // Continue in while loop until we reach end time in increments of dt
    while (m_currTime < SIMULATION_DURATION) {

      m_currTime += dt;
      for (auto &station : m_unloadStations) {
        station.update();
      }

      // Process all trucks in simulation
      for (auto &truck : m_trucks) {
        truck.update();

        // If a truck is ready to unload and there is an available unload
        // station then assign the truck to the unload station
        if ((truck.getState() == TruckState::UNLOADING ||
             truck.getState() == TruckState::IDLE) &&
            truck.hasStation() == false) {
          for (auto &station : m_unloadStations) {
            if (station.getTruckInStation() == nullptr) {
              // If truck was waiting and found station, move its state
              // accordingly
              if (truck.getState() == TruckState::IDLE) {
                truck.setState(TruckState::UNLOADING);
              }
              truck.setHasStation(true);
              station.setTruckInStation(&truck);
              break;
            } else {
              truck.setState(TruckState::IDLE);
            }
          }
        }
      }
    }
    printTruckStats();
    printStationStats();
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

    for (auto &station : m_unloadStations) {
      std::cout << "Station " << station.getId() << " stats:" << std::endl;
      std::cout << "Time occupied: " << station.getTimeOccupied() << std::endl;

      std::unordered_map<uint32_t, float> truckTimes = station.getTruckTimes();

      for (auto &truck : truckTimes) {
        std::cout << "Truck ID: " << truck.first << " time: " << truck.second
                  << std::endl;
      }
    }
  }
};