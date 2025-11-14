# Game Controller Input - Tutorial

This example demonstrates how to handle real-time gamepad/controller input using the helios framework. It covers polling device state, reading button and analog axis values, and using that input to drive visual feedback in a scene.

## What You'll Learn

- Setting up the helios `InputManager` for gamepads
- Registering and polling controller devices
- Reading button states (pressed/released)
- Reading analog stick and trigger values
- Structuring a scene to visualize input
- Updating scene node transforms and materials based on input

## Prerequisites

- helios framework properly built and installed
- A compatible USB or Bluetooth game controller (e.g., Xbox, PlayStation)
- Basic understanding of C++ and the helios scene graph

## Project Structure

```
game_controller_input/
├── main.cpp           # Main application code
├── resources/
│   ├── simple_vertex_shader.vert
│   └── simple_fragment_shader.frag
└── README.md         # This file
```

## Step-by-Step Tutorial

### 1. Application and Input Manager Setup

We start by creating an application and getting a reference to its `InputManager`.

```cpp
// From main()
const auto app = GLFWFactory::makeOpenGLApp(
    "helios - Game Controller Input Demo"
);
auto win = dynamic_cast<GLFWWindow*>(app->current());

InputManager& inputManager = app->inputManager();
```

### 2. Registering the Gamepad

Before we can receive input, we must tell the `InputManager` which controller(s) to listen to.

```cpp
// From main()
unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);
assert(mask == 1 && "unexpected return value for mask");
```
- `Gamepad::ONE` specifies that we want to use the first connected controller.
- You can register multiple controllers using a bitmask (e.g., `Gamepad::ONE | Gamepad::TWO`).

### 3. Scene Setup for Visualization

We create a scene with several simple shapes (circles, rectangles) that will represent the controller's buttons, sticks, and triggers. Each shape is a `SceneNode` with a `Renderable` component.

```cpp
// From main()

// Create a shared shader and material properties
auto shader = std::make_shared<OpenGLShader>(...);
auto buttonMaterialPropsOverride = MaterialPropertiesOverride(...);

// Create a renderable for a button (e.g., a circle)
auto buttonRenderableA = std::make_shared<Renderable>(
    circlePrototype, buttonMaterialPropsOverride
);

// Add the renderable to a scene node
const auto buttonGroupNode_ptr = scene.addNode(std::make_unique<SceneNode>());
const auto buttonA_ptr = buttonGroupNode_ptr->addChild(
    std::make_unique<SceneNode>(buttonRenderableA)
);
```
This process is repeated for all visual elements (B, X, Y buttons, sticks, etc.).

### 4. The Main Render Loop

The core logic resides in the `while` loop, which runs every frame.

```cpp
// From main()
while (!win->shouldClose()) {
    // 1. Process window events
    app->eventManager().dispatchAll();

    // 2. Poll for new input
    inputManager.poll(0.0f);

    // 3. Get the state of our registered gamepad
    const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);

    // 4. Update visuals based on the new state
    updateButton(*buttonA_ptr, gamepadState.buttonA(), ...);
    // ... update other elements ...

    // 5. Render the scene
    auto snapshot = scene.createSnapshot(...);
    auto renderPass = RenderPassFactory::getInstance().buildRenderPass(snapshot);
    app->renderingDevice().render(renderPass);

    // 6. Swap buffers
    win->swapBuffers();
}
```

**Loop Breakdown:**
1. **Event Processing**: Handles window events like closing.
2. **Input Polling**: `inputManager.poll()` updates the internal state of all registered devices. This is a crucial step.
3. **Get State**: `inputManager.gamepadState()` provides a snapshot of the controller's current state for that frame.
4. **Update Logic**: We use the `gamepadState` object to modify our scene.
5. **Rendering**: The updated scene is drawn to the screen.

### 5. Reading Input and Applying Updates

The `GamepadState` object provides simple accessors for all inputs.

**Reading Buttons:**
The `buttonA()` method returns `true` if the 'A' button is currently held down. We pass this boolean to a helper function.

```cpp
// In the main loop
updateButton(*buttonA_ptr, gamepadState.buttonA(), buttonMaterialPropsOverride);
updateButton(*buttonB_ptr, gamepadState.buttonB(), buttonMaterialPropsOverride);
```

**The `updateButton` Helper:**
This function scales the node and changes its opacity to give visual feedback.
```cpp
void updateButton(
    helios::scene::SceneNode& button,
    const bool pressed,
    helios::rendering::model::config::MaterialPropertiesOverride& originalOverride
) {
    // Scale up when pressed
    button.scale(helios::math::vec3f(SCALING_FACTOR * (pressed ? 1.2f : 1.0f)));
    
    // Make it opaque when pressed, semi-transparent otherwise
    if (button.renderable()->hasMaterialOverride()) {
        // ... logic to change material alpha ...
    }
}
```

**Reading Analog Axes:**
The `leftX()`, `leftY()`, and `triggerLeft()` methods return a `float` value, typically between `-1.0` and `1.0` for sticks and `0.0` to `1.0` for triggers.

```cpp
// In the main loop

// Get stick and trigger values
const float leftTrigger = gamepadState.triggerLeft();
const float axisLeftX  = gamepadState.leftX();

// Use values to transform scene nodes
stickAxisLeft_ptr->scale(helios::math::vec3f(gamepadState.left() * 0.5f));
triggerLeftNode_ptr->scale(helios::math::vec3f(triggerBaseScale[0] * leftTrigger, ...));
```

## Building and Running

From the project root directory:

```bash
# Configure the build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build --config Release

# Run the example (Windows)
.\build\examples\game_controller_input\Release\main.exe
```

## Expected Output

A window will appear displaying a layout of shapes. When you press buttons, move the sticks, or pull the triggers on your connected controller, the corresponding shapes will change size and color in real-time. Press `ESC` to exit.

## Key helios Concepts

- **`InputManager`**: The central hub for all input devices.
- **`registerGamepads()`**: Tells the `InputManager` which controllers to track.
- **`poll()`**: The function that must be called every frame to capture new input data.
- **`gamepadState()`**: Retrieves a read-only snapshot of a controller's state for the current frame.
- **`GamepadState` Object**: Provides simple accessors like `buttonA()`, `leftX()`, `triggerRight()` to get input values.

## Troubleshooting

### Controller Not Detected
- Ensure your controller is properly connected via USB or Bluetooth before starting the application.
- Verify the controller works in other games or system utilities.
- Check the console output for any messages from GLFW about controller detection.

### No Visual Feedback
- Confirm that `inputManager.poll()` is being called inside the main loop.
- Make sure you are requesting the correct gamepad index with `inputManager.gamepadState(Gamepad::ONE)`.

## License

This example is part of the helios framework and is distributed under the MIT License.




