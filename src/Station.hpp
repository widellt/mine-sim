#pragma once
#include "Truck.hpp"
#include <unordered_map>

class Station {
private:
  int m_id;
  float m_dt;
  float m_timeOccupied = 0.0f;
  float m_timeRemaining = 0.0f;
  Truck *m_truckInStation = nullptr;

  std::unordered_map<uint32_t, float> m_truckTimes;

public:
  Station(int stationId, float dt) : m_id(stationId), m_dt(dt) {}

  int getId() const { return m_id; }

  float getTimeOccupied() const { return m_timeOccupied; }

  Truck *getTruckInStation() const { return m_truckInStation; }

  std::unordered_map<uint32_t, float> getTruckTimes() const {
    return m_truckTimes;
  }

  void setTruckInStation(Truck *truck) {
    m_truckInStation = truck;
    m_timeRemaining =
        Truck::UNLOAD_TIME / m_dt; // Initialize with unload time and count down
  }

  // Main function to call on each timestep for individual stations
  void update() {
    if (m_truckInStation != nullptr) {
      // Update how long a specific truck has been in a station
      // and total time station has been occupied

      // If the truck is done unloading, remove it from the station and update
      // its state
      if (m_truckInStation->getUnloadTimeLeft() <= 0) {
        m_truckInStation->setHasStation(false);
        m_truckInStation = nullptr;
      } else {
        m_truckTimes[m_truckInStation->getId()] += m_dt;
        m_timeOccupied += m_dt;
      }
    }
  }
};
