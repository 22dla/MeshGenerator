# MeshGenerator

MeshGenerator is a tool for generating 3D meshes based on XYZ input data. This project allows you to read points from an input XYZ file, create a mesh, and save it to an output file. Calculations are based on the Delaunay triangulation method.

## Usage

To use MeshGenerator, follow these steps:

1. Compile the project using CMake:

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```
2. You can install the project:

   ```shell
   mkdir build
   cd build
   cmake -DCMAKE_INSTALL_PREFIX=/путь/к/установочной/директории ..
   make
   make install   
3. Or you can use Cmake GUI:
![image](https://github.com/22dla/MeshGenerator/assets/39338008/0638f8a0-b305-4b12-8552-a5fd3cdc9169)

4. Run MeshGenerator, specifying the input and output files:

   ```shell
    ./MeshGenerator -i input.xyz -o output.xyz
   ```
Where input.xyz is your input data file, and output.xyz is the name of the file where the generated mesh will be saved.

## Dependencies
To build and use MeshGenerator, the following dependencies are required:

- CMake
- C++11 or higher

## Authors
Volkov Evgeny
volkov22dla@gmail.com
