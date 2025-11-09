# helios

![](./docs/logo.png)

**helios** is a custom game ~~engine~~ framework built from first principles, written in C++.

### Project Overview
helios is an educational project to explore the intricacies of game engine architecture. Each component is built from first principles: The primary goal is to apply and analyze modern software engineering in a performance-critical domain while implementing the mathematical building blocks of 2D and 3D graphics.

The engine strives for a modern **C++23** module-based architecture, providing a clean and robust dependency graph for fast compilation. 

The initial rendering backend is implemented using **OpenGL**.

### Development Philosophy
The development process emphasizes understanding both the mathematical foundations and the rationale behind established patterns, such as trade-offs between indirect and direct data storage or the division of labor between CPU-side culling and GPU-side clipping.


### Directory layout

The project is structured as follows:

```
.
├── benchmarks/
├── docs/
├── examples/
├── include/
│   ├── ext/
│   └── helios/
├── src/
│   ├── ext/
│   └── helios/
└── tests/
```

#### `benchmarks`
Contains various source files providing benchmarks for runtime behavior of selected classes and modules.

#### `docs`
Contains documentation for helios source files and usage. 

#### `examples`
Contains the source code for selected examples of scenes and gameplay elements rendered with helios.

#### `include`
The root source folder for interface units directly related to the helios project. The `include`-folder is further separated into

 - `include/helios`: Module interface units for the helios framework
 - `include/ext`: Extensions and platform specific interface units, providing interfaces based on the helios framework using vendor libraries such as **glfw** and **opengl**.

#### `src`
Module implementation units for interface units found in `include/ext` and `include/helios`.

#### `tests`
Various test cases for selected classes and functionality.


## Bibliography
[Vri20] de Vries, Joey: Learn OpenGL (2020), Kendall & Wells
\
[RTR] Akenine-Möller, Tomas and Haines, Eric and Hoffman, Naty: Real-Time Rendering (2018), A. K. Peters, Ltd.  Real Time Rendering
\
[Gre19] Gregory, J: Game Engine Architecture, 3rd ed. (2018). A K Peters/CRC Press. 
\
[She07] Sherrod, Allen: Data Structures and Algorithms for Game Developers (2007), Charles River Media, Boston, Massachusetts 
\
[HDMS+14]: Hughes, John F. and van Dam, Andries and McGuire, Morgan and Sklar, David F. and Foley, James D. and Feiner, Steven K. and Akeley, Kurt: Computer Graphics - Principles and Practice (2014), Addison-Wesley Educational 
\
[KSS17]: Kessenich, John and Sellers, Graham and Shreiner, Dave: The OpenGL Programming Guide (2017), Addison Wesley
\
[Str22]: Stroustrup, Bjarne: A Tour of C++ (2022), Addison-Wesley

[PJH23]: Pharr, Matt and Jakob, Wenzel and Humphreys, Greg: Physically Based Rendering, 4th ed. (2023), MIT Press 