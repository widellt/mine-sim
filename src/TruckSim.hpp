// Simplified simulation class for trucks only
#include <vector>
#include "Truck.hpp"

class TruckSimulation {
    private:
        std::vector<Truck> trucks;
        static const float TRAVEL_TIME = 1800; //s
        static const float UNLOAD_TIME = 300; //s

    public:
        TruckSimulation(int numTrucks){
            trucks = std::vector<Truck>(numTrucks);
        }

        void simulate(){
            // TODO
        }

};