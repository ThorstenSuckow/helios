# helios

![](./docs/logo.png)

**helios** is a custom game engine built from first principles, written in C++.

## Project Overview
helios is an educational project to explore the intricacies of game engine architecture. Each component is built from first principles: The primary goal is not to create a commercial product, but to apply and analyze modern software engineering in a performance-critical domain while implementing the mathematical building blocks of 2D and 3D graphics.

The engine strives for a modern C++20 module-based architecture, providing a clean and robust dependency graph for fast compilation. 

The initial rendering backend is implemented using **OpenGL**.

## Development Philosophy
The development process emphasizes understanding both the mathematical foundations and the rationale behind established patterns, such as trade-offs between indirect and direct data storage or the division of labor between CPU-side culling and GPU-side clipping.
