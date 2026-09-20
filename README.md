# Drone Simulator 3D

A 3D drone flight simulator built with C++ and [raylib](https://www.raylib.com/). Simulate drone physics with thrust, drag, and altitude control in a 3D environment.

## Features

- 3D flight with realistic drag-based physics
- Follow camera with mouse orbit control
- Ground collision (drone can't go below Y = 0)
- Support for loading custom 3D drone models (.obj)
- Simple and extensible codebase

## Dependencies

- [raylib](https://www.raylib.com/) — graphics and input
- [MSYS2](https://www.msys2.org/) with MinGW-w64 (Windows)
- g++ (GCC 13+)

## Installation

### Windows (MSYS2)

1. Install [MSYS2](https://www.msys2.org/)
2. Open the MSYS2 MinGW64 terminal and install raylib and gcc:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-raylib
   pacman -S mingw-w64-x86_64-gcc
   ```
3. Add `C:\msys64\mingw64\bin` to your Windows system PATH

## Building

From the project directory in VS Code (or any terminal with g++ on PATH):

```bash
g++ main.cpp -o drone_sim -IC:/msys64/ucrt64/include -LC:/msys64/ucrt64/lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

## Running

```bash
./drone_sim
```

Make sure `drone.obj` is in the same directory as the executable if you want the 3D drone model to load.

## Controls

| Key | Action |
|-----|--------|
| W | Thrust forward (negative Z) |
| S | Thrust backward (positive Z) |
| A | Thrust left |
| D | Thrust right |
| Space | Ascend |
| Left Ctrl | Descend |
| Left Mouse + Drag | Orbit camera around drone |

## Project Structure

```
DroneSim/
├── main.cpp        # Main simulation loop and rendering
├── drone.obj       # 3D drone model (provide your own)
└── README.md
```

## Physics Model

The drone uses a simple velocity-based physics model:

- WASD and Space/Ctrl apply thrust to velocity components (vx, vy, vz)
- Each frame, velocity is multiplied by a drag coefficient (0.95) to simulate air resistance
- Position is updated each frame based on current velocity
- The drone is clamped to Y ≥ 0 (ground level)

## Roadmap

- [ ] Heading/rotation with model orientation
- [ ] Telemetry overlay (x, y, z, velocity)
- [ ] Wind and external forces(?)
- [ ] Waypoint navigation
- [ ] PID autopilot controller
- [ ] Larger world with terrain

## Additional Comments

So far a very fun project to work on! Planning on making more commits soon. 