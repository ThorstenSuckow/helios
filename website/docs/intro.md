---
sidebar_position: 1
---

# Welcome to helios

**helios** is a custom game framework built from first principles, written in modern C++23.

## What is helios?

helios is an educational project designed to explore the intricacies of game engine architecture. Each component is built from first principles with a focus on:

- **Modern C++23** module-based architecture
- **Clean dependency management** with fast compilation times
- **Flexible rendering pipeline** with OpenGL backend
- **Component-based design** for maintainability

## Quick Start

Get started with helios in just a few steps:

```bash
# Clone the repository
git clone https://github.com/thorstensuckow/helios.git
cd helios

# Configure with CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run an example
cd build/examples/simple_cube_rendering/Release
./simple_cube_rendering
```

## Features

- ✅ **Modern C++23**: Module-based architecture with clean dependency management
- ✅ **Scene Graph**: Hierarchical scene management with transform propagation
- ✅ **Rendering Pipeline**: Flexible render passes with batching support
- ✅ **Input System**: Unified input handling for keyboard, mouse, and gamepad
- ✅ **Math Library**: Vector, matrix, and transform operations for 3D graphics
- ✅ **OpenGL Backend**: Initial rendering implementation using OpenGL 4.5+
- ✅ **Cross-Platform**: Windows, Linux, and macOS support
- ✅ **Extensible**: Plugin architecture for different rendering backends

## Learning Path

:::tip Code Examples
Code snippets in this documentation are often simplified to focus on specific concepts. For complete, compilable examples including all necessary setup (shaders, materials, meshes), please refer to the **[examples/](https://github.com/thorstensuckow/helios/tree/main/examples)** directory in the repository.
:::

1. **[Getting Started](./getting-started)** - Set up your development environment
2. **[Your First Application](./examples/simple-cube)** - Create a rotating cube
3. **[Core Concepts](./core-concepts/scene-graph)** - Understand helios architecture
4. **[API Reference](./api/overview)** - Detailed API documentation

## Project Philosophy

The development process emphasizes understanding both the mathematical foundations and the rationale behind established patterns, such as trade-offs between indirect and direct data storage or the division of labor between CPU-side culling and GPU-side clipping.

## Academic Context

This project originated as a semester project for a computer science degree. The design principles, architecture, and prototypical implementation are documented in:

- **[Suc25]** Suckow-Homberg, Thorsten: *helios: Design and prototypical implementation of a C++ game framework* (2025), [ResearchGate](https://www.researchgate.net/publication/397445662_helios_Design_and_prototypical_implementation_of_a_C_game_framework)

## Community

▸ [GitHub Discussions](https://github.com/thorstensuckow/helios/discussions) - Ask questions, share ideas<br />
▸ [Issues](https://github.com/thorstensuckow/helios/issues) - Report bugs or request features<br />
▸ Contact: [thorsten@suckow-homberg.de](mailto:thorsten@suckow-homberg.de)

## License

helios is licensed under the **MIT License**. See the [LICENSE](https://github.com/thorstensuckow/helios/blob/main/LICENSE) file for details.

