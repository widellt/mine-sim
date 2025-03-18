#pragma once
#include <random>

// Enum to represent the simplified state of a mining truck
enum class TruckState {
  MINING,
  TRAVELING_TO_STATION,
  UNLOADING,
  TRAVELING_TO_SITE,
  IDLE
};

class Truck {

  int m_id;
  float m_dt;
  TruckState m_state;

  float m_miningTimeLeft;
  float m_miningTimeTotal = 0;
  float m_unloadTimeLeft;
  float m_unloadTimeTotal = 0;
  float m_travelTimeLeft;
  float m_travelTimeTotal = 0;
  float m_idleTimeTotal = 0;
  uint32_t m_tripsCompleted;

  bool m_isUnloading;
  bool m_isTravelingToSite;
  bool m_isTravelingToStation;
  bool m_isMining;
  bool m_hasStation;
  static constexpr float HRS_TO_SECS = 3600.0f;
  static constexpr float MINE_TIME_MAX = 5.0f;
  static constexpr float MINE_TIME_MIN = 1.0f;

public:
  static constexpr float UNLOAD_TIME = 300.0f;
  static constexpr float TRAVEL_TIME = 1800.0f;

  Truck(int truckId, float dt)
      : m_id(truckId), m_dt(dt), m_state(TruckState::MINING),
        m_isUnloading(false), m_isTravelingToSite(false),
        m_isTravelingToStation(false), m_isMining(true), m_hasStation(false) {
    // Initialize with random mining time (1-5 hours)
    m_miningTimeLeft = (int)getRandomMiningTime() / m_dt;
  }

  int getId() const { return m_id; }
  /// \brief Retrieves the current state of the truck
  TruckState getState() const { return m_state; }

  /// \brief Sets the state of the truck
  void setState(TruckState newState) { m_state = newState; }

  /// \brief Retrieves the amount of time left until the truck is done mining
  float getMiningTimeLeft() const { return m_miningTimeLeft; }

  /// \brief Retrieves the total amount of time the truck has spent mining
  float getMiningTimeTotal() const { return m_miningTimeTotal; }
  
  /// \brief Retrieves the amount of time left until the truck is done unloading
  float getUnloadTimeLeft() const { return m_unloadTimeLeft; }

  /// \brief Retrieves the total amount of time the truck has spent unloading
  float getUnloadTimeTotal() const { return m_unloadTimeTotal; }

  /// \brief Retrieves the total amount of time the truck has spent traveling
  float getTravelTimeTotal() const { return m_travelTimeTotal; }

  /// \brief Retrieves the total amount of time the truck has spent waiting for a station
  float getIdleTimeTotal() const { return m_idleTimeTotal; }

  /// \brief Retrieves the total number of trips the truck has completed
  uint32_t getTripsCompleted() const { return m_tripsCompleted; }

  /// \brief Retrieves the total amount of time the truck has spent idle
  bool hasStation() const { return m_hasStation; }

  /// \brief Sets the state of the truckif it has a station
  void setHasStation(bool newState) { m_hasStation = newState; }

  /// \brief Generates a random mining time
  float getRandomMiningTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> miningDist(MINE_TIME_MIN, MINE_TIME_MAX);

    return (miningDist(gen) * HRS_TO_SECS);
  }

  /// \brief Main function to call on each timestep for individual trucks
  void update() {

    switch (m_state) {
    case TruckState::MINING:
      m_miningTimeLeft -= m_dt;
      m_miningTimeTotal += m_dt;
      if (m_miningTimeLeft <= 0) {
        m_state = TruckState::TRAVELING_TO_STATION;
        m_isMining = false;
        m_isTravelingToStation = true;
        m_miningTimeLeft = (int)getRandomMiningTime() / m_dt;
        m_travelTimeLeft = TRAVEL_TIME / m_dt;
      }
      break;
    case TruckState::TRAVELING_TO_STATION:
      m_travelTimeLeft -= m_dt;
      m_travelTimeTotal += m_dt;
      if (m_travelTimeLeft <= 0) {
        m_state = TruckState::UNLOADING;
        m_isTravelingToStation = false;
        m_isUnloading = true;
        m_unloadTimeLeft = UNLOAD_TIME / m_dt;
      }
      break;
    case TruckState::UNLOADING:
      m_unloadTimeLeft -= m_dt;
      m_unloadTimeTotal += m_dt;
      if (m_unloadTimeLeft <= 0) {
        m_state = TruckState::TRAVELING_TO_SITE;
        // We want the unload station to set the transition from unload to
        // traveling to the site
        m_isUnloading = false;
        m_isTravelingToSite = true;
        m_travelTimeLeft = TRAVEL_TIME / m_dt;
      }
      break;
    case TruckState::TRAVELING_TO_SITE:
      m_travelTimeLeft -= m_dt;
      m_travelTimeTotal += m_dt;
      if (m_travelTimeLeft <= 0) {
        m_state = TruckState::MINING;
        m_isTravelingToSite = false;
        m_isMining = true;
      }
      break;
    case TruckState::IDLE:
      m_idleTimeTotal += m_dt;
      break;
    }
  }
};