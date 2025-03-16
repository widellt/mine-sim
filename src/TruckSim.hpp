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
        uint32_t m_numTrucks;
        uint32_t m_numStations;
        double m_currTime = 0;
        static constexpr float dt = 10.0f; //s
        static constexpr float SIMULATION_DURATION = 259200.0f / dt;

    public:
        TruckSim(int numTrucks, int numStations) : 
            m_numTrucks(numTrucks),
            m_numStations(numStations) 
        {
            // Initialize trucks and unload stations
            for (int i = 0; i < numTrucks; i++){
                m_trucks.push_back(Truck(i, dt));
            }
            for (int i = 0; i < numStations; i++){
                m_unloadStations.push_back(Station(i));
            }
        }

        void simulate(){

            std::cout << "Simulating " << m_numTrucks << " trucks" << std::endl;
            // Continue in while loop until we reach end time
            while (m_currTime < SIMULATION_DURATION){

                m_currTime+=dt;
                // Process all trucks in simulation
                for (uint32_t i = 0; i < m_numTrucks; i++){
                    m_trucks[i].updateState();
                }
            }
            printTruckStats();
        }

        void printTruckStats(){
            std::cout << "Truck Stats:" << std::endl;
            for (uint32_t i = 0; i < m_numTrucks; i++){
                std::cout << "Truck ID " << m_trucks[i].getId() << std::endl;
                std::cout << "Mining Time Total: " << m_trucks[i].getMiningTimeTotal() * dt << std::endl;
                std::cout << "Unload Time Total: " << m_trucks[i].getUnloadTimeTotal() * dt << std::endl;
                std::cout << "Travel Time Total: " << m_trucks[i].getTravelTimeTotal() * dt << std::endl;

                std::cout << "Total Time: " << (m_trucks[i].getMiningTimeTotal() * dt) + (m_trucks[i].getUnloadTimeTotal() * dt) + (m_trucks[i].getTravelTimeTotal() * dt) << std::endl;
                std::cout << std::endl;
            }
        }

};