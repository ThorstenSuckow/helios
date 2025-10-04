#include <iostream>
#include <cstdlib>


import helios.math.types;

import helios.ext.glfw.app.GLFWFactory;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.app.GLFWRAIIGuard;
import helios.ext.glfw.window.GLFWWindow;

import helios.input.InputManager;
import helios.input.types.Key;
import helios.rendering.model.MaterialData;
import helios.rendering.model.MeshData;
import helios.rendering.asset.shape.basic.Cube;
import helios.util.io.BasicStringFileReader;

import helios.ext.opengl.rendering.model.OpenGLShader;
import helios.ext.opengl.rendering.model.OpenGLMaterial;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.model.OpenGLNode;

using namespace helios::ext::glfw::app;
using namespace helios::rendering::model;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::rendering::asset::shape::basic;
using namespace helios::input::types;
using namespace helios::ext::glfw::window;
using namespace helios::util::io;

int main() {

    auto glfwGuard = GLFWRAIIGuard();

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Simple Cube Renderer"
    );

    auto win = dynamic_cast<GLFWWindow*>(app->current());


    // get the InputManager
    helios::input::InputManager& inputManager = app->inputManager();

    const auto basicStringFileReader = BasicStringFileReader();

    // model
    auto shader_ptr = std::make_shared<OpenGLShader>(
        "./resources/cube.vert",
        "./resources/cube.frag",
        basicStringFileReader
    );
    auto materialData_ptr = std::make_shared<MaterialData>(shader_ptr);
    auto material_ptr = std::make_unique<OpenGLMaterial>(materialData_ptr);

    auto cube = Cube{};
    auto meshData = std::make_shared<const MeshData>(cube);
    auto mesh_ptr = std::make_unique<OpenGLMesh>(meshData);
    auto node = OpenGLNode(std::move(mesh_ptr), std::move(material_ptr));



    while (!win->shouldClose()) {
        app->eventManager().dispatchAll();

        inputManager.poll(0.0f);

        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
            win->setShouldClose(true);
        }

        app->renderingDevice().beginRenderPass();

        // model
        node.draw();

        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}