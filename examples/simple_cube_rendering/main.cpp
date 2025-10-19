#include <cstdlib>
#include <iostream>
#include <numbers>


import helios.math.types;

import helios.ext.glfw.app.GLFWFactory;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindow;

import helios.input.InputManager;
import helios.input.types.Key;

import helios.rendering.RenderQueue;
import helios.rendering.RenderPassFactory;
import helios.rendering.model.MaterialData;
import helios.rendering.model.MeshData;
import helios.rendering.asset.shape.basic.Cube;

import helios.util.io.BasicStringFileReader;

import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.rendering.model.Material;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.OpenGLRenderable;

import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.rendering.shader.UniformSemantics;

import helios.scene.Scene;
import helios.scene.SceneNode;
import helios.scene.CullNoneStrategy;
import helios.scene.Camera;

import helios.math;

using namespace helios::ext::glfw::app;
using namespace helios::rendering;
using namespace helios::rendering::model;
using namespace helios::rendering::shader;
using namespace helios::ext::opengl::rendering;
using namespace helios::ext::opengl::rendering::shader;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::rendering::asset::shape::basic;
using namespace helios::input::types;
using namespace helios::ext::glfw::window;
using namespace helios::util::io;
using namespace helios::scene;
using namespace helios::math;


int main() {

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

    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    if (!uniformLocationMap->set(UniformSemantics::WorldMatrix, 1)) {
        // intentionally left empty
    }
    shader_ptr->setUniformLocationMap(std::move(uniformLocationMap));

    auto material_ptr = std::make_unique<Material>(shader_ptr);

    auto cube = Cube{};


    auto meshData = std::make_shared<const MeshData>(cube);
    auto mesh_ptr = std::make_shared<OpenGLMesh>(meshData);
    auto cubeRenderable = std::make_shared<OpenGLRenderable>(
        std::move(mesh_ptr), std::move(material_ptr)
    );

    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto rootSceneNode = std::make_unique<helios::scene::SceneNode>();
    auto scene = std::make_unique<helios::scene::Scene>(std::move(frustumCullingStrategy));
    if (!scene->addNode(std::move(rootSceneNode))) {/*intentionally left empty*/}
    auto cubeSceneNode = std::make_unique<helios::scene::SceneNode>(std::move(cubeRenderable));
    auto* cubeNode = scene->addNode(std::move(cubeSceneNode));
    cubeNode->scale(vec3f(0.5f, 0.5f, 0.5f));
    // camera
    auto cam = std::make_unique<helios::scene::Camera>();
    auto* cameraNode = scene->addNode(std::move(cam));
    if (!cameraNode) {/*intentionally left empty*/}
    const auto* camera = static_cast<const Camera*>(cameraNode);



    float degrees = 0.0f;
    float rad = 0.0f;
    while (!win->shouldClose()) {
        app->eventManager().dispatchAll();

        inputManager.poll(0.0f);

        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
            win->setShouldClose(true);
        }
        rad = helios::math::radians(degrees+=2.25);
        cubeNode->rotate(helios::math::rotate(
        helios::math::mat4f::identity(),
            rad,
        helios::math::vec3f(0.4f, 0.6, 0.2f)
        ));
        degrees = degrees > 360 ? 0 : degrees;

        // create the snapshot of the scene for the rendering pass
        const auto& snapshot = scene->createSnapshot(*camera);
        auto renderPass = RenderPassFactory::getInstance().buildRenderPass(snapshot);
        app->renderingDevice().render(renderPass);

        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}