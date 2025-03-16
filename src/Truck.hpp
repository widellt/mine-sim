#pragma once
#include <random>

// Enum to represent the simplified state of a mining truck
enum class TruckState {
    MINING,
    TRAVELING_TO_STATION,
    UNLOADING, // Simplified unloading without stations
    TRAVELING_TO_SITE
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
    uint32_t m_tripsCompleted;

    bool m_isUnloading;
    bool m_isTravelingToSite;
    bool m_isTravelingToStation;
    bool m_isMining;
    static constexpr float TRAVEL_TIME = 1800.0f;
    static constexpr float UNLOAD_TIME = 300.0f;
    static constexpr float HRS_TO_SECS = 3600.0f;
    static constexpr float MINE_TIME_MAX = 5.0f;
    static constexpr float MINE_TIME_MIN = 1.0f;

    
public:
    Truck(int truckId, float dt) : 
        m_id(truckId), 
        m_dt(dt),
        m_state(TruckState::MINING), 
        m_isUnloading(false), 
        m_isTravelingToSite(false), 
        m_isTravelingToStation(false),
        m_isMining(true)
    {
        // Initialize with random mining time (1-5 hours)
        m_miningTimeLeft = getRandomMiningTime();
    }

    int getId() const {
        return m_id;
    }

    TruckState getState() const {
        return m_state;
    }

    void setState(TruckState newState) {
        m_state = newState;
    }

    float getMiningTimeTotal() const {
        return m_miningTimeTotal;
    }

    float getUnloadTimeTotal() const {
        return m_unloadTimeTotal;
    }

    float getTravelTimeTotal() const {
        return m_travelTimeTotal;
    }

    float getRandomMiningTime() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> miningDist(MINE_TIME_MIN, MINE_TIME_MAX);

        return (miningDist(gen) * HRS_TO_SECS);
    }

    void updateState() {
        // Update the state of the truck based on its current state and reset random mining time

        switch(m_state) {
            case TruckState::MINING:
                m_miningTimeLeft -= m_dt;
                m_miningTimeTotal += m_dt;
                if (m_miningTimeLeft <= 0) {
                    m_state = TruckState::TRAVELING_TO_STATION;
                    m_isMining = false;
                    m_isTravelingToStation = true;
                    m_miningTimeLeft = getRandomMiningTime() / m_dt;
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
        }
    }


};