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
├── src/
│   ├── helios/
│   ├── ext/
│   └── impl/
└── tests/
```

#### `benchmarks`
Contains various source files providing benchmarks for runtime behavior of selected classes and modules.

#### `docs`
Contains documentation for helios source files and usage. 

#### `examples`
Contains the source code for selected examples of scenes and gameplay elements rendered with helios.

#### `src`
The root source folder for interface and implementation units directly related with the helios project. The `src`-folder is further separated into

 - `src/helios`: Module interface units for teh helios framework
 - `src/ext`: Extensions and platform specific interface units, providing concrete interfaces for various vendor libraries such as **glfw** and **opengl**
 - `src/impl`: Module implementation units for interface units found in `ext` and `helios`.

#### `tests`
Various test cases for selected classes anf funcionality.



