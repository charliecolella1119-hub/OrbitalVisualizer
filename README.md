# Orbital Visualizer

A real-time molecular orbital visualization engine built in C++ with SFML.  
This project renders quantum-inspired atomic orbitals as interactive 3D point clouds with rotation, zoom, depth shading, and phase coloring.

---

## Features

- Real-time 3D orbital rendering
- Interactive camera controls
- Depth sorting and perspective projection
- Phase-colored orbital lobes
- Auto-rotation mode
- Adjustable point density
- Zoom and pan controls
- Real spherical harmonic angular forms for d orbitals
- Spherical-coordinate orbital sampling
- Adjustable perspective camera distance
- H2 bonding and antibonding molecular orbitals
- Multiple orbital visualizations:
  - 1s
  - 2s
  - 2p_x
  - 2p_y
  - 2p_z
  - 3d_x²-y²
  - 3d_xy
  - 3d_z² cross section

---

## Controls

| Key | Action |
|-----|--------|
| 1-8 | Select atomic orbitals |
| 9 | H2 bonding orbital |
| 0 | H2 antibonding orbital |
| WASD | Pan view |
| Arrow Keys / Q / E | Rotate view |
| Mouse Wheel | Zoom |
| + / - | Increase/decrease point density |
| [ / ] | Adjust H2 bond length |
| R | Reset view |
| T | Toggle auto-rotate |
| Esc | Quit |

---

## Technologies Used

- C++
- SFML 3
- 3D point-cloud rendering
- Custom perspective projection
- Depth sorting algorithms

---

## Build Instructions

### Requirements

- C++17 or newer
- SFML 3

### macOS Build

```bash
g++ -std=c++17 main.cpp Orbital.cpp Renderer.cpp -o main \
-I/opt/homebrew/include \
-L/opt/homebrew/lib \
-lsfml-graphics \
-lsfml-window \
-lsfml-system
```
## Run 

./main

## Demo

![Orbital Visualizer Screenshot](images/orbital.png)


