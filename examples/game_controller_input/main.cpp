#include <cassert>
#include <cstdlib>
#include <iostream>
#include <print>


import helios.math.types;

import helios.ext.glfw.app.GLFWFactory;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindow;

import helios.input.InputManager;
import helios.input.GamepadState;
import helios.input.types.Key;
import helios.input.types.Gamepad;

using namespace helios::input;
using namespace helios::input::types;
using namespace helios::ext::glfw::app;
using namespace helios::ext::glfw::window;

int main() {

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Simple Cube Renderer"
    );

    auto win = dynamic_cast<GLFWWindow*>(app->current());


    // get the InputManager
    InputManager& inputManager = app->inputManager();

    unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);
    assert(mask == 1 && "unexpected return value for mask");

    while (!win->shouldClose()) {
        app->eventManager().dispatchAll();

        inputManager.poll(0.0f);

        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
            win->setShouldClose(true);
        }


        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const float leftTrigger = gamepadState.triggerLeft();
        const float rightTrigger = gamepadState.triggerRight();

        const float axisLeftX  = gamepadState.leftX();
        const float axisLeftY  = gamepadState.leftY();
        const float axisRightX = gamepadState.rightX();
        const float axisRightY = gamepadState.rightY();

        std::cout << std::format(
            "\rCTRL: LFT ({0:.2f}, {1:.2f}) - RGT ({2:.2f}, {3:.2f}) - TR [{4:.2f}] [{5:.2f}]",
            axisLeftX, axisLeftY, axisRightX, axisRightY,
            leftTrigger, rightTrigger
        ) << std::flush;


        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}