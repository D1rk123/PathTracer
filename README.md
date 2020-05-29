# PathTracer

This is a hobby project in which I implemented Monte Carlo path tracing for physics based rendering.

## Current Features
  * Unidirectional path tracing
  * Multithreaded execution based on the C++11 threading library
  * 2 Types of geometry (spheres and infinite planes)
  * 3 Materials (diffuse colored surfaces, mirrors and glass)
  * 2 Types of light sources (point lights with next event estimation or emissive surfaces)
  * Saving images as HDR exr files and LDR ppm files


![Example Render](https://raw.githubusercontent.com/D1rk123/PathTracer/master/ExampleImages/glassMirrorDiffuseExample.png)

## Building and running the code
At the moment there is no user interface yet, so to change the scene you have to recompile the code. A visual studio project is provided in this repository. Two libraries are used (GLM and TinyEXR) but both are header only so they don't need to be compiled separately. All code in this repository is multiplatform.
