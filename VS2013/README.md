# ArrayFire Visual Studio 2013 Template Projects
This repository contains 2 solution project configurations.

## ArrayFire-VS2013
This is the standard project pre-configured for CPU, CUDA and OpenCL for x64.

This project allows you to choose `Release` or `Debug` from the Solution
Configurations and either of `CPU`, `CUDA` or `OpenCL` backend from the
Solution Platforms.

You can use this project when you are not writing your own CUDA or OpenCL
kernels. As long as you are only using C++ and ArrayFire code and not depending
on native CUDA or OpenCL code, this project will allow maximum portability.

## ArrayFire-CUDA
This project uses an NVCC project which allows for native CUDA code.

By default this project links with CUDA 8.0. To use other toolkits, go to
`Project -> Build Customizations`. Uncheck CUDA 8.0 and select the alternative
CUDA toolkit you wish to use.

This project allows you to choose `Release` or `Debug` from the Solution
Configurations, but are limited to CUDA as the Solution Platform.

You can use this project to write your own CUDA kernels and use other CUDA APIs.
