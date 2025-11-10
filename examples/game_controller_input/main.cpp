#include <cassert>
#include <cstdlib>
#include <iostream>
#include <print>
#include <glad/gl.h>


import helios.math.types;

import helios.ext.glfw.app.GLFWFactory;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindow;

import helios.input.InputManager;
import helios.input.GamepadState;
import helios.input.types.Key;
import helios.input.types.Gamepad;

// math
import helios.math.types;

// io
import helios.util.io.BasicStringFileReader;

// rendering
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.rendering.shader.UniformSemantics;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.OpenGLRenderable;
import helios.rendering.model.MaterialInstance;
import helios.rendering.model.Material;
import helios.rendering.model.config.MeshConfig;
import helios.rendering.model.config.MaterialProperties;
import helios.rendering.model.config.MaterialPropertiesOverride;
import helios.rendering.model.config.PrimitiveType;
import helios.rendering.RenderPassFactory;
import helios.ext.opengl.rendering.OpenGLDevice;

// model data
import helios.rendering.asset.shape.basic.Circle;
import helios.rendering.asset.shape.basic.Line;
import helios.rendering.asset.shape.basic.Rectangle;


// scene
import helios.scene.Scene;
import helios.scene.SceneNode;
import helios.scene.CullNoneStrategy;
import helios.scene.Camera;

using namespace helios::input;
using namespace helios::input::types;
using namespace helios::ext::glfw::app;
using namespace helios::ext::glfw::window;

int main() {

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Game Controller Input Demo"
    );

    auto win = dynamic_cast<GLFWWindow*>(app->current());



    // helper
    auto stringFileReader = helios::util::io::BasicStringFileReader();


    // ------------------------------
    // Rendering: Rendering Device, Renderable and Uniforms
    // ------------------------------
    auto renderingDevice = dynamic_cast<helios::ext::opengl::rendering::OpenGLDevice&>(app->renderingDevice());

    // shader configuration
    auto shader = std::make_shared<helios::ext::opengl::rendering::shader::OpenGLShader>(
        "./resources/simple_vertex_shader.vert",
        "./resources/simple_fragment_shader.frag",
        stringFileReader
    );
    auto uniformLocationMap = std::make_unique<helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap>();
    bool uniformAssigned    = uniformLocationMap->set(helios::rendering::shader::UniformSemantics::WorldMatrix, 1);
    assert(uniformAssigned && "could not assign the world matrix uniform");
    uniformAssigned    = uniformLocationMap->set(helios::rendering::shader::UniformSemantics::MaterialBaseColor, 2);
    assert(uniformAssigned && "could not assign the world matrix uniform");
    shader->setUniformLocationMap(std::move(uniformLocationMap));


    // material configs
    auto circleMaterialProps = helios::rendering::model::config::MaterialProperties(
        helios::math::vec4f(1.0f, 0.0f, 1.0f, 0.5f),
        0.0
    );
    auto lineMaterialProps = helios::rendering::model::config::MaterialProperties(
        helios::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f),
        0.0
    );
    auto circleMaterialProps_shared = std::make_shared<helios::rendering::model::config::MaterialProperties>(circleMaterialProps);
    auto lineMaterialProps_shared   = std::make_shared<helios::rendering::model::config::MaterialProperties>(lineMaterialProps);

    auto circleShape      = helios::rendering::asset::shape::basic::Circle();
    auto circleMeshConfig = std::make_shared<const helios::rendering::model::config::MeshConfig>(
        circleShape.primitiveType()
   );
    auto circleMesh             = std::make_shared<helios::ext::opengl::rendering::model::OpenGLMesh>(circleShape, circleMeshConfig);
    auto circleMaterial         = std::make_shared<helios::rendering::model::Material>(shader, circleMaterialProps_shared);
    auto circleMaterialInstance = std::make_shared<helios::rendering::model::MaterialInstance>(circleMaterial);
    auto circleRenderable       = std::make_shared<helios::ext::opengl::rendering::OpenGLRenderable>(
        circleMesh, circleMaterialInstance
    );

    auto lineShape      = helios::rendering::asset::shape::basic::Line();
    auto lineMeshConfig = std::make_shared<const helios::rendering::model::config::MeshConfig>(
        lineShape.primitiveType()
    );
    auto lineMesh             = std::make_shared<helios::ext::opengl::rendering::model::OpenGLMesh>(lineShape, lineMeshConfig);
    auto lineMaterial         = std::make_shared<helios::rendering::model::Material>(shader, lineMaterialProps_shared);
    auto lineMaterialInstance = std::make_shared<helios::rendering::model::MaterialInstance>(lineMaterial);
    auto lineRenderable       = std::make_shared<helios::ext::opengl::rendering::OpenGLRenderable>(
        lineMesh, lineMaterialInstance
    );

    auto rectangleShape       = helios::rendering::asset::shape::basic::Rectangle();
    auto rectangleShapeConfig = std::make_shared<const helios::rendering::model::config::MeshConfig>(
        rectangleShape.primitiveType()
    );
    auto rectangleMesh             = std::make_shared<helios::ext::opengl::rendering::model::OpenGLMesh>(rectangleShape, rectangleShapeConfig);
    auto rectangleMaterialInstance = std::make_shared<helios::rendering::model::MaterialInstance>(lineMaterial);
    auto rectangleRenderable       = std::make_shared<helios::ext::opengl::rendering::OpenGLRenderable>(
        rectangleMesh, rectangleMaterialInstance
    );

    // ------------------------------
    //  Scene
    // ------------------------------
    auto cullingStrategy = std::make_unique<helios::scene::CullNoneStrategy>();
    auto scene = helios::scene::Scene(std::move(cullingStrategy));

    // camera
    const auto camera_ptr = scene.addNode(std::make_unique<helios::scene::Camera>());
    assert(camera_ptr != nullptr && "unexpected nullptr for circleNode");

    // scene nodes
    // left stick
    float scalingFactor = 1.0f/5.0f;

    const auto stickLeftNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(circleRenderable));
    assert(stickLeftNode_ptr != nullptr && "unexpected nullptr for stickLeftNode_ptr");
    auto stickAxisLeft_ptr = stickLeftNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(lineRenderable));
    assert(stickAxisLeft_ptr != nullptr && "unexpected nullptr for stickAxisLeft_ptr");
    stickLeftNode_ptr->scale(helios::math::vec3f(scalingFactor, scalingFactor, 0.0f));
    stickLeftNode_ptr->translate(helios::math::vec3f(-0.5f, 0.0f, 0.0f));

    // right stick
    const auto stickRightNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(circleRenderable));
    assert(stickRightNode_ptr != nullptr && "unexpected nullptr for stickRightNode_ptr");
    auto stickAxisRight_ptr = stickRightNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(lineRenderable));
    assert(stickAxisRight_ptr != nullptr && "unexpected nullptr for stickAxisRight_ptr");
    stickRightNode_ptr->scale(helios::math::vec3f(scalingFactor, scalingFactor, 0.0f));
    stickRightNode_ptr->translate(helios::math::vec3f(0.0f, 0.0f, 0.0f));

    // left/right trigger
    const auto triggerLeftNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(rectangleRenderable));
    assert(triggerLeftNode_ptr != nullptr && "unexpected nullptr for triggerLeftNode_ptr");
    const auto triggerRightNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(rectangleRenderable));
    assert(triggerRightNode_ptr != nullptr && "unexpected nullptr for triggerRightNode_ptr");

    // ------------------------------
    //  Input
    // ------------------------------
    InputManager& inputManager = app->inputManager();
    // register the gamepads
    unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);
    assert(mask == 1 && "unexpected return value for mask");

    // ------------------------------
    //  Logging
    // ------------------------------
    // disable logging
    helios::util::log::LogManager::getInstance().enableLogging(false);

    // game loop
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
            "\rCTRL: LFT ({0:.2f}, {1:.2f}) - RGT ({2:.2f}, {3:.2f}) - TR [{4:.2f}] [{5:.2f}] - A[{6}] B[{7}] X[{8}] Y[{9}] ST[{10}] BCK[{11}] G[{12}] LB[{13}] RB[{14}] LT[{15}] RT[{16}] DU[{17}] DR[{18}] DD[{19}] DL[{20}]",
            axisLeftX, axisLeftY, axisRightX, axisRightY,
            leftTrigger, rightTrigger,
            static_cast<int>(gamepadState.buttonA()),
            static_cast<int>(gamepadState.buttonB()),
            static_cast<int>(gamepadState.buttonX()),
            static_cast<int>(gamepadState.buttonY()),
            static_cast<int>(gamepadState.buttonStart()),
            static_cast<int>(gamepadState.buttonBack()),
            static_cast<int>(gamepadState.buttonGuide()),
            static_cast<int>(gamepadState.buttonLeftBumper()),
            static_cast<int>(gamepadState.buttonRightBumper()),
            static_cast<int>(gamepadState.buttonLeftThumb()),
            static_cast<int>(gamepadState.buttonRightThumb()),
            static_cast<int>(gamepadState.buttonDpadUp()),
            static_cast<int>(gamepadState.buttonDpadRight()),
            static_cast<int>(gamepadState.buttonDpadDown()),
            static_cast<int>(gamepadState.buttonDpadLeft())
        ) << std::flush;

        // scaling
        // sticks:
        stickAxisLeft_ptr->scale(helios::math::vec3f(gamepadState.left()));
        stickAxisRight_ptr->scale(helios::math::vec3f(gamepadState.right()));
        // trigger
        // @note the Rectangle shape has a default height of 2.0f and will grow and shrink equally in both
        // x and y direction. This makes scaling and translating more complicated than it should be -
        // the lib is in need of initializing a scenenode with a "fixed" translation, and subsequently
        // translate the node by a specific amount, using "translateBy()".
        triggerLeftNode_ptr->scale(helios::math::vec3f(scalingFactor * 0.5f, leftTrigger*scalingFactor, 0.0f));
        triggerLeftNode_ptr->translate(helios::math::vec3f(-0.85f, leftTrigger*scalingFactor, 0.0f));

        triggerRightNode_ptr->scale(helios::math::vec3f(scalingFactor * 0.5f, rightTrigger*scalingFactor, 0.0f));
        triggerRightNode_ptr->translate(helios::math::vec3f(0.5f, rightTrigger*scalingFactor, 0.0f));


        auto snapshot = scene.createSnapshot(dynamic_cast<helios::scene::Camera&>(*camera_ptr));
        auto renderPass = helios::rendering::RenderPassFactory::getInstance().buildRenderPass(snapshot);
        app->renderingDevice().render(renderPass);

        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}