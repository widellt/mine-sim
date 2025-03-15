// Simplified simulation class for trucks only
#include <vector>
#include <queue>
#include <iostream>
#include "Truck.hpp"
#include "Station.hpp"

class TruckSim {
    private:

        // Let's assume one truck and one unload station to begin with
        std::vector<Truck> m_trucks;
        std::vector<Station> m_unloadStations;
        std::queue<Truck> m_waitingLine;
        double m_currTime = 0;
        static constexpr float TRAVEL_TIME = 1800.0f; //s, 30 min to mining site from unload station
        static constexpr float UNLOAD_TIME = 300.0f; //s, 5 min to unload
        static constexpr float SIMULATION_DURATION = 259200.0f;
        static constexpr float dt = 0.1f; //s

    public:
        TruckSim(int numTrucks, int numStations) {


            // Initialize trucks and unload stations
            for (int i = 0; i < numTrucks; i++){
                m_trucks.push_back(Truck(i));
            }

            for (int i = 0; i < numStations; i++){
                m_unloadStations.push_back(Station(i));
            }
            
        }

        void simulate(){

            // Continue in while loop until we reach end time
            while (m_currTime < SIMULATION_DURATION){

                m_currTime+=dt;

                // Process all trucks in simulation
                // m_trucks->updateState(dt);

            }

        }

};