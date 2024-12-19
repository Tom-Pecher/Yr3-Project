
import traci
import os

# Path to the SUMO binary
SUMO_BINARY = "sumo-gui"  # Use "sumo" for non-GUI version

def run_simulation():
    # Start SUMO with the given configuration
    traci.start([SUMO_BINARY, "-c", "intersection.sumocfg"])

    step = 0
    while step < 100:  # Run the simulation for 100 steps
        traci.simulationStep()
        vehicles = traci.vehicle.getIDList()
        print(f"Simulation step {step}: Vehicles in simulation = {len(vehicles)}")
        step += 1

    traci.close()

if __name__ == "__main__":
    # Ensure SUMO_HOME is set in the environment
    if "SUMO_HOME" not in os.environ:
        raise EnvironmentError("Please set SUMO_HOME in your environment variables.")

    run_simulation()
