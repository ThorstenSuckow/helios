#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>


import helios.util;
import helios.glfw.application;
import helios.rendering.opengl;
import helios.platform.input;
import helios.glfw.input;
import helios.platform.input.core;
import helios.platform.input.types;
import helios.platform.window.core;
import helios.glfw.window;


using namespace helios::glfw::window;
using namespace helios::rendering::opengl;
using namespace helios::util;

unsigned int load(const GLFWWindow& window) {
    std::string vertexShader;
    std::string fragmentShader;

    IOUtil::readInto("./resources/cube.frag", fragmentShader);
    IOUtil::readInto("./resources/cube.vert", vertexShader);

    return OpenGLShaderCompiler::compile(
        vertexShader.c_str(), fragmentShader.c_str()
    );
}

void render (const GLFWWindow& window, const unsigned int progId) {


    glUseProgram(progId);


    float vertices[] = {
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f,  -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f,  -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f
       };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,

        4, 5, 7,
        5, 6, 7
    };


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_LOOP, 12, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


int main() {

    auto glfwGuard = helios::glfw::application::GLFWRAIIGuard();

    const auto app = helios::glfw::application::GLFWFactory::makeOpenGLApp(
        "helios - Simple Cube Renderer"
    );

    auto win = dynamic_cast<helios::glfw::window::GLFWWindow*>(app->current());


    // get the InputManager
    helios::platform::input::InputManager& inputManager = app->inputManager();

    const unsigned int progId = load(*win);

    while (!win->shouldClose()) {
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

        if (inputManager.isKeyPressed(helios::platform::input::Key::ESC)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
            std::ignore = win->setShouldClose(true);
        }

        //update();
        render(*win, progId);

        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}