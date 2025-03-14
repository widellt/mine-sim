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
    
public:
    Truck(int truckId) : m_id(truckId) {
        // Initialize with random mining time (1-5 hours)
        
    }

    int getId() const {
        return m_id;
    }


};