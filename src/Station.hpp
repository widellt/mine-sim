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
  // Constructor
  // \param stationId The identifier of the station.
  // \param dt The time step of the simulation.
  Station(int stationId, float dt) : m_id(stationId), m_dt(dt) {}

  /// \brief Retrieves the unique identifier of the station.
  int getId() const { return m_id; }

  /// \brief Retrieves the amount of time the station has been occupied.
  float getTimeOccupied() const { return m_timeOccupied; }

  /// \brief Returns the pointer to the truck currently in the station
  Truck *getTruckInStation() const { return m_truckInStation; }

  /// \brief Retrieves all the times a specific truck has been in the station
  std::unordered_map<uint32_t, float> getTruckTimes() const {
    return m_truckTimes;
  }

  /// \brief Sets the pointer to the truck currently in the station
  void setTruckInStation(Truck *truck) {
    m_truckInStation = truck;
    m_timeRemaining =
        Truck::UNLOAD_TIME / m_dt; // Initialize with unload time and count down
  }

  /// \brief Updates the state of the station per timestep
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
