# Particle Filter for Robot Localization

## Project Overview
This project implements a particle filter algorithm to estimate the position (x, y) and orientation (theta) of a robot in a grid-based environment. The robot's movement commands and sensor observations are used to update the particle weights, leading to a more accurate estimation of its location over time.

## Features
- Particle Filter: Implements the core particle filter algorithm with sampling, update, and normalization steps.
- Grid Environment: Represents the robot's environment as a 100x100 grid where each cell is either bright (0) or dark (1).
- Sensor Observation: Simulates a sensor that can determine whether the robot is in a bright or dark cell.
- Movement Commands: Supports forward movement ("F") and rotation ("R") commands.

## Code Structure
- `PFStruct`: Struct to represent a particle with its position (x, y, theta), and weight.
- `Command`: Struct to store robot movement commands (steps, theta).
- `update` Function: Updates a particle's weight based on the sensor observation (Z) and its position in the environment.
- `normalize` Function: Normalizes the weights of all particles in an array to sum up to 1.
- `init` Function: Initializes an array of particles with random positions and weights.
- `sample` Function: Implements sampling with replacement using the cumulative distribution function (CDF) of particle weights.
- `ParticleFilter` Function: Core function performing the particle filter algorithm:
  - Sampling particles based on weights.
  - Updating particle positions based on movement commands.
  - Boundary enforcement.
  - Weight updating based on sensor observations.
  - Weight normalization.
- `createEnvironment` Function: Creates a grid-based environment with bright/dark values based on a checkerboard pattern.
- `checkObservation` Function: Determines sensor observation based on robot position and environment.
- `readCSVLine` Function: Reads robot movement commands and sensor observations from a CSV file ("robot.txt").
- `report` Function: Calculates mean and variance of particle positions, comparing to actual robot location for debugging.
- `main` Function: Drives the overall process:
  - Initializes particle and environment arrays.
  - Reads commands and observations from "robot.txt".
  - Iteratively calls ParticleFilter and updates particles.
  - Compares estimated and actual robot positions using report.

## Files
- `main.cpp`: Contains main logic for particle filter algorithm.
- `robot.txt`: CSV file with robot's movement commands and sensor observations.

## Usage
- Ensure you have a C++ compiler installed.
- Copy provided code into a C++ source file (e.g., particle_filter.cpp).
- Create a CSV file named "robot.txt" with format: x,y,translation,rotation.
- Compile code using C++ compiler (e.g., g++ particle_filter.cpp -o particle_filter).
- Run compiled executable (./particle_filter).
- Program reads robot commands from "robot.txt", performs particle filter updates, and reports estimated robot position compared to actual location for each iteration.

## Future Enhancements
- Implement error handling for invalid data in CSV file.
- Explore different particle initialization strategies.
- Introduce sensor noise for a more realistic environment.
- Visualize estimated robot location and particle distribution on grid environment.
