# 3D Perlin Noise Map with Marching Cubes Algorithm

This project demonstrates the rendering of a 3D Perlin noise map using the Marching Cubes algorithm. The rendering is done using the Raylib library in C.

<img src="https://github.com/JessevanVuuren/MarchingCubes/blob/master/img/img_1.png?raw=true" alt="isolated" width="400"/>
<img src="https://github.com/JessevanVuuren/MarchingCubes/blob/master/img/img_2.png?raw=true" alt="isolated" width="400"/>


## Overview

The project generates a 3D terrain based on Perlin noise. The Marching Cubes algorithm is used to triangulate the generated noise data, allowing the 3D terrain to be rendered efficiently.

### Key Features

- **Perlin Noise:** A classic algorithm for generating smooth, continuous random values.
- **Marching Cubes:** A surface generation algorithm used to draw a mesh from 3D scalar fields.
- **Raylib Integration:** Raylib is used for rendering and managing the 3D environment.

## Links and Resources

- [Polygonising a Scalar Field by Paul Bourke](https://paulbourke.net/geometry/polygonise/)
- [Perlin Noise Algorithm by Riad T.](https://rtouti.github.io/graphics/perlin-noise-algorithm)

These resources were crucial in understanding and implementing the algorithms used in this project.

## Usage

Raylib is already included in this repo. To build this project run `$ make` followed by `.\build\main.exe`
