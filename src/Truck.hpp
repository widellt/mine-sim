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
    TruckState m_state;

    uint32_t m_miningTime;
    uint32_t m_unloadTime;
    uint32_t m_travelTime;
    uint32_t m_tripsCompleted;

    bool m_isUnloading;
    bool m_isTravellingToSite;
    bool m_isTravellingToStation;
    bool m_isMining;

    
public:
    Truck(int truckId) : m_id(truckId), 
        m_state(TruckState::TRAVELING_TO_SITE), 
        m_miningTime(0), 
        m_unloadTime(0), 
        m_travelTime(0), 
        m_tripsCompleted(0), 
        m_isUnloading(false), 
        m_isTravellingToSite(false), 
        m_isTravellingToStation(false),
        m_isMining(true)
    {
        // Initialize with random mining time (1-5 hours)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> miningDist(1.0, 5.0);
        
        m_miningTime = miningDist(gen);
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

    uint32_t getMiningTime() const {
        return m_miningTime;
    }


};