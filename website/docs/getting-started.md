---
sidebar_position: 2
---

# Installation

This guide will help you set up helios and run your first example.

## Prerequisites

> [!IMPORTANT] 
> Before you begin, ensure your system meets all requirements listed in the [Prerequisites](./prerequisites.md) guide. This includes:
>
> - **C++23 compatible compiler** (MSVC 2022, GCC 13+, Clang 16+)
> - **CMake 4.0+**
> - **OpenGL 4.5+** support
> - **Git** (optional for cloning)
>
> For detailed installation instructions for your platform, see [Prerequisites](./prerequisites.md).

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/thorstensuckow/helios.git
cd helios
```

### 2. Configure the Build

#### Windows (Visual Studio)

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

#### Linux / macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

### 3. Build the Project

#### Windows

```powershell
cmake --build build --config Release
```

#### Linux / macOS

```bash
cmake --build build
```

### 4. Run Tests (Optional)

```bash
cd build
ctest -C Release --output-on-failure
```

## Running Tests

helios includes a comprehensive test suite to verify functionality. Tests are built automatically when you build the project.

### Running All Tests

#### Windows (Visual Studio)

```powershell
cd build
ctest -C Release --output-on-failure
```

#### Linux / macOS

```bash
cd build
ctest --output-on-failure
```

### Running Specific Tests

```bash
# Run tests matching a pattern
ctest -R "Math" --output-on-failure

# Run a specific test
ctest -R "Vector3Test" --output-on-failure

# List all available tests
ctest -N
```

### Test Output

- **`--output-on-failure`**: Shows detailed output only for failed tests
- **`-V` or `--verbose`**: Shows all test output (useful for debugging)
- **`--rerun-failed`**: Re-runs only tests that failed in the previous run

### Example Test Session

```bash
$ cd build
$ ctest -C Release --output-on-failure
Test project F:/thorstensuckow/helios/build
    Start 1: Vector3Test
1/5 Test #1: Vector3Test ......................   Passed    0.02 sec
    Start 2: Matrix4Test
2/5 Test #2: Matrix4Test ......................   Passed    0.03 sec
    Start 3: TransformTest
3/5 Test #3: TransformTest ....................   Passed    0.01 sec
    Start 4: SceneGraphTest
4/5 Test #4: SceneGraphTest ...................   Passed    0.04 sec
    Start 5: RenderingTest
5/5 Test #5: RenderingTest ....................   Passed    0.05 sec

100% tests passed, 0 tests failed out of 5

Total Test time (real) =   0.16 sec
```

### Debugging Failed Tests

If a test fails, use verbose mode to see detailed output:

```bash
ctest -R "FailedTestName" -V
```

Or run the test executable directly:

```bash
# Windows
./tests/helios/math/Debug/Vector3Test.exe

# Linux/macOS
./tests/helios/math/Vector3Test
```

## Running Your First Example

After building, navigate to the example directory:

```bash
cd build/examples/simple_cube_rendering/Release
./simple_cube_rendering
```

You should see a window with a rotating yellow wireframe cube. Press `ESC` to exit.

## Next Steps

- **[Simple Cube Tutorial](./examples/simple-cube)** - Learn how the cube example works
- **[Core Concepts](./core-concepts/scene-graph)** - Understand helios architecture
- **[API Reference](./api/overview)** - Explore the full API

## Troubleshooting

### Compilation Errors

**CMake version too old:**
```
Ensure you have CMake 4.0 or higher installed.
```

**C++23 not supported:**
```
Update your compiler to the minimum required version.
```

### Runtime Errors

**Shader files not found:**
```
Make sure you run the example from the build directory where resources are copied.
```

**OpenGL context errors:**
```
Ensure your GPU drivers are up to date and support OpenGL 4.5+.
```

## Getting Help

▸ [GitHub Discussions](https://github.com/thorstensuckow/helios/discussions)<br />
> [Report an Issue](https://github.com/thorstensuckow/helios/issues)

