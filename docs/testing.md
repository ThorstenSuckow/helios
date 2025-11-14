# Testing

helios includes a comprehensive test suite to verify functionality. This guide covers how to run tests, interpret results, and debug failures.

## Running Tests

Tests are built automatically when you build the project with CMake.

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

You can run tests matching a specific pattern:

```bash
# Run all math-related tests
ctest -R "Math" --output-on-failure

# Run a specific test
ctest -R "Vector3Test" --output-on-failure

# List all available tests without running them
ctest -N
```

### Test Configuration

#### Build Configuration

Tests are built in the same configuration as the main project:

```bash
# Debug build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
cd build
ctest -C Debug --output-on-failure

# Release build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cd build
ctest -C Release --output-on-failure
```

## Test Output Options

CTest provides several options to control output:

| Flag | Description |
|------|-------------|
| `--output-on-failure` | Show detailed output only for failed tests |
| `-V` or `--verbose` | Show all test output (useful for debugging) |
| `--rerun-failed` | Re-run only tests that failed in the previous run |
| `-N` | List all tests without running them |
| `-R <regex>` | Run tests matching the regex pattern |
| `-E <regex>` | Exclude tests matching the regex pattern |
| `--timeout <seconds>` | Set a timeout for each test |

## Example Test Session

Here's what a successful test run looks like:

```bash
$ cd build
$ ctest -C Release --output-on-failure
Test project F:/thorstensuckow/helios/build
    Start 1: Vector3Test
1/12 Test #1: Vector3Test ......................   Passed    0.02 sec
    Start 2: Vector4Test
2/12 Test #2: Vector4Test ......................   Passed    0.02 sec
    Start 3: Matrix4Test
3/12 Test #3: Matrix4Test ......................   Passed    0.03 sec
    Start 4: TransformTest
4/12 Test #4: TransformTest ....................   Passed    0.01 sec
    Start 5: CameraTest
5/12 Test #5: CameraTest .......................   Passed    0.02 sec
    Start 6: SceneGraphTest
6/12 Test #6: SceneGraphTest ...................   Passed    0.04 sec
    Start 7: RenderingTest
7/12 Test #7: RenderingTest ....................   Passed    0.05 sec
    Start 8: MaterialTest
8/12 Test #8: MaterialTest .....................   Passed    0.03 sec
    Start 9: InputTest
9/12 Test #9: InputTest ........................   Passed    0.02 sec
   Start 10: GuidTest
10/12 Test #10: GuidTest .........................   Passed    0.01 sec
   Start 11: FileReaderTest
11/12 Test #11: FileReaderTest ...................   Passed    0.02 sec
   Start 12: LoggerTest
12/12 Test #12: LoggerTest .......................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 12

Total Test time (real) =   0.28 sec
```

## Debugging Failed Tests

### Verbose Output

If a test fails, use verbose mode to see detailed output:

```bash
ctest -R "FailedTestName" -V
```

### Running Test Executables Directly

You can also run test executables directly for more control:

#### Windows

```powershell
# Debug build
.\tests\helios\math\Debug\Vector3Test.exe

# Release build
.\tests\helios\math\Release\Vector3Test.exe
```

#### Linux / macOS

```bash
# Run test directly
./tests/helios/math/Vector3Test

# With valgrind for memory leak detection
valgrind ./tests/helios/math/Vector3Test
```

### Understanding Test Output

Failed tests will show:
- **Expected vs. Actual values** - What the test expected and what it got
- **Stack trace** - Where the failure occurred in the code
- **Test name** - Which specific test case failed

Example of a failed test:

```
Test project F:/thorstensuckow/helios/build
    Start 1: Vector3Test
1/1 Test #1: Vector3Test ......................***Failed    0.02 sec

Running main() from gtest_main.cc
[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 5 tests from Vector3
[ RUN      ] Vector3.Addition
[       OK ] Vector3.Addition (0 ms)
[ RUN      ] Vector3.Subtraction
[       OK ] Vector3.Subtraction (0 ms)
[ RUN      ] Vector3.Multiplication
F:/thorstensuckow/helios/tests/helios/math/Vector3Test.cpp:42: Failure
Expected equality of these values:
  result.x
    Which is: 2.5
  2.0
    Which is: 2

[  FAILED  ] Vector3.Multiplication (1 ms)
```

## Test Structure

helios tests are organized by module:

```
tests/
├── helios/
│   ├── math/           # Math library tests
│   │   ├── Vector3Test.cpp
│   │   ├── Vector4Test.cpp
│   │   ├── Matrix4Test.cpp
│   │   └── TransformTest.cpp
│   ├── scene/          # Scene graph tests
│   │   ├── SceneTest.cpp
│   │   └── CameraTest.cpp
│   ├── rendering/      # Rendering tests
│   │   ├── MaterialTest.cpp
│   │   └── RenderPassTest.cpp
│   ├── input/          # Input system tests
│   │   └── InputManagerTest.cpp
│   └── util/           # Utility tests
│       ├── GuidTest.cpp
│       └── LoggerTest.cpp
└── ext/                # External library tests
    └── glfw/
```

## Continuous Integration

Tests run automatically on every push and pull request via GitHub Actions. The workflow tests on:

- **Windows** (MSVC) - Debug & Release
- **Linux** (GCC 13 & Clang 17) - Debug & Release  
- **macOS** (Apple Clang) - Debug & Release

See the [Test Workflow](.github/workflows/tests.yml) for the complete CI configuration.

### Workflow Triggers

The test workflow runs when:
- Code is pushed to `main` or `develop` branches
- Pull requests are created targeting `main` or `develop`
- Changes are made to source files (`src/`, `include/`, `tests/`)
- CMake configuration is modified

### Viewing Test Results

1. Go to the [Actions tab](https://github.com/thorstensuckow/helios/actions) in the GitHub repository
2. Click on a workflow run to see detailed results
3. Failed tests will show error logs and stack traces
4. Test artifacts are uploaded for failed builds

## Writing Tests

If you're contributing to helios, please include tests for new features and bug fixes. See the [Contributing Guide](../contributing/guide) for guidelines on writing tests.

### Test Framework

helios uses **Google Test (gtest)** for unit testing. Example:

```cpp
#include <gtest/gtest.h>
#include "helios/math/Vector3.h"

TEST(Vector3Test, Addition) {
    helios::math::Vector3f a(1.0f, 2.0f, 3.0f);
    helios::math::Vector3f b(4.0f, 5.0f, 6.0f);
    
    auto result = a + b;
    
    EXPECT_FLOAT_EQ(result.x, 5.0f);
    EXPECT_FLOAT_EQ(result.y, 7.0f);
    EXPECT_FLOAT_EQ(result.z, 9.0f);
}
```

## Troubleshooting

### Common Issues

**Tests not found:**
```bash
# Make sure you're in the build directory
cd build
ctest -N  # List all tests
```

**CMake configuration errors:**
```bash
# Reconfigure and rebuild
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

**Missing test executables:**
```bash
# Rebuild the project
cmake --build build --config Release --target all
```

### Platform-Specific Issues

#### Windows

- Ensure you specify the configuration (`-C Release` or `-C Debug`)
- Test executables are in `build/tests/.../Release/` or `Debug/`

#### Linux

- Use `ldd` to check for missing shared libraries:
  ```bash
  ldd ./tests/helios/math/Vector3Test
  ```

## See Also

- [Getting Started](./getting-started) - Initial setup and build instructions
- [Contributing Guide](../contributing/guide) - How to contribute tests
- [API Reference](../api/overview) - Detailed API documentation

