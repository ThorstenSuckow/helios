#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

#include "../../.build/cmake_build_release/_deps/glfw-src/include/GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const unsigned short width = 800;
    const unsigned short height = 600;

    GLFWwindow* window = glfwCreateWindow(
        width, height,
        "Simple Cube Rendering", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int gl_ver = gladLoadGL(glfwGetProcAddress);
    if (gl_ver == 0) {
       std::fprintf(stderr, "Failed to load OpenGL\n");
       return EXIT_FAILURE;
    }
    std::printf(
        "OpenGL %d.%d loaded\n",
        GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver)
    );

    glViewport(0,0,800, 600);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    return EXIT_SUCCESS;
}