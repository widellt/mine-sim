# mine-sim
The purpose of this simulator is to provide a statistical analysis of an N number of mining trucks that are capable of offloading at an M number of unload stations over a 72 hour period.

 # Operation Details:
 ● Thereare (n) mining trucks and (m) mining unload stations.
 ● Mining trucks can spend a random duration between 1 to 5 hours mining at the sites.
 ● It takes a mining truck 30 minutes to travel between a mining site and an unload station.
 ○ Assume all trucks are empty at a mining site when the simulation starts.
 ● Unloading the mined Helium-3 at a station takes 5 minutes.
 ● Trucks are assigned to the first available unload station. If all stations are occupied, trucks
 queue at the station with the shortest wait time and remain in their chosen queue.


# BUILDING 

# Build in Debug mode (default)
./build.sh

# Build in Release mode (optimized)
./build.sh --release

# Build and run unit tests
./build.sh --unit-test

# Clean build and rebuild
./build.sh --clean

# Build and run the simulation with parameters
./build.sh --run-sim -- -t 10 -s 3 -v

# Build dependencies
CMake 3.10
GDB
G++ 11.4.0
clang-format
