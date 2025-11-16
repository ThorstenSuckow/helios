# Prerequisites

This document outlines the requirements needed to build and run **helios**. Ensure all prerequisites are met before attempting to build the project.

## System Requirements

### Operating System
- **Windows** 10/11 (x64)
- **Linux** (Ubuntu 20.04+, Fedora 35+, or equivalent)
- **macOS** 11+

### Processor
- Support for C++23 features and module compilation
- x64 architecture recommended

## Required Software

### CMake
- **Minimum version**: 4.0
- **Installation**:
  - **Windows**: Download from [cmake.org](https://cmake.org/download/) or use package managers (Chocolatey, Scoop)
  - **Linux**: `sudo apt install cmake` (Ubuntu) or equivalent for your distribution
  - **macOS**: `brew install cmake`

### Compiler

The project requires a **C++ compiler with comprehensive C++23 module support**.

#### Recommended Compilers

##### Windows
- **MSVC 2022** (Visual Studio 2022 v17.4+)
  - Supports C++23 modules out of the box
  - Download: [Visual Studio Community](https://visualstudio.microsoft.com/downloads/)
  - Required workload: "Desktop development with C++"

##### Linux
- **GCC 13+** (with C++23 module support)
  - `sudo apt install g++-13` (Ubuntu)
  - `sudo dnf install gcc-toolset-13` (Fedora)
- **Clang 16+** (with C++23 module support)
  - `sudo apt install clang-16` (Ubuntu)
  - `sudo dnf install clang-tools-extra-16` (Fedora)

##### macOS
- **Clang 16+** (bundled with Xcode 14.3+)
  - Install Xcode Command Line Tools: `xcode-select --install`
  - Or install full Xcode from the App Store

#### Compiler Version Check

Verify your compiler version:

```bash
# MSVC
cl.exe /?

# GCC
g++ --version

# Clang
clang++ --version
```

### Build Tool

Choose one:

- **Ninja** (recommended for faster builds)
  - `sudo apt install ninja-build` (Linux)
  - `brew install ninja` (macOS)
  - Windows: Download from [ninja releases](https://github.com/ninja-build/ninja/releases)

- **Visual Studio 2022** (Windows only, includes its own build system)

- **GNU Make** (typically pre-installed on Linux/macOS)

### OpenGL

- **OpenGL 4.5+** support required
- GPU driver with OpenGL 4.5+ support
- Most modern GPUs (NVIDIA, AMD, Intel) support this

## Optional Software

### Testing & Quality Assurance

- **CTest** (included with CMake)
- **clang-format** (for code formatting)
  - `sudo apt install clang-format` (Linux)
  - `brew install clang-format` (macOS)
  - Included with Visual Studio (Windows)

### Documentation

- **Doxygen** (for API documentation)
  - Download from [doxygen.org](https://www.doxygen.nl/download.html)

### Version Control

- **Git** (recommended, required for cloning the repository)
  - Download from [git-scm.com](https://git-scm.com/)

## Verification

### Step 1: Verify CMake
```bash
cmake --version
```
Expected output: `cmake version 4.0` or higher

### Step 2: Verify Compiler
```bash
# MSVC (Windows)
cl.exe /?

# GCC/Clang
g++ --version
# or
clang++ --version
```
Ensure version meets the minimum requirement for C++23 modules.

### Step 3: Verify C++23 Module Support

Create a test file `test_modules.cpp`:
```cpp
// Test C++23 module compilation
int main() {
    return 0;
}
```

Compile with your compiler:
```bash
# MSVC
cl.exe /std:c++latest /EHsc test_modules.cpp

# GCC
g++-13 -std=c++23 test_modules.cpp

# Clang
clang++-16 -std=c++23 test_modules.cpp
```

If compilation succeeds, your compiler supports C++23.

## Platform-Specific Setup

### Windows (MSVC)

1. Install **Visual Studio 2022 Community Edition**
2. During installation, select **"Desktop development with C++"**
3. Install **CMake** (or use Visual Studio's integrated CMake support)
4. Install **Ninja** (optional, for faster builds)

```bash
# Verify setup
cmake --version
cl.exe /?
```

### Linux (Ubuntu 20.04+)

```bash
# Install dependencies
sudo apt update
sudo apt install cmake g++-13 ninja-build git

# Verify
cmake --version
g++-13 --version
```

### Linux (Fedora)

```bash
# Install dependencies
sudo dnf install cmake gcc-toolset-13 ninja-build git

# Enable GCC 13
scl enable gcc-toolset-13 bash

# Verify
cmake --version
g++-13 --version
```

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install dependencies via Homebrew
brew install cmake ninja

# Verify
cmake --version
clang++ --version
```

## Troubleshooting

### "CMake Error: cmake version required is 4.0 and above"
- Update CMake to version 4.0 or higher

### "C++23 module support not available"
- Update your compiler to a version that supports C++23 modules (see recommended versions above)

### "OpenGL headers not found"
- On Linux, install development headers: `sudo apt install libgl1-mesa-dev`

### Build fails with module compilation errors
- Ensure your compiler is up-to-date and supports C++23
- Clear your build directory: `rm -rf build` and reconfigure

## Additional Resources

- [C++23 Standard Reference](https://en.cppreference.com/w/cpp/23)
- [CMake Documentation](https://cmake.org/documentation/)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [OpenGL Documentation](https://www.khronos.org/opengl/)

## Next Steps

Once all prerequisites are installed, proceed with the [Quick Start](../README.md#quick-start) guide.

