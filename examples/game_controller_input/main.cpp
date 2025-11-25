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

// math
import helios.math.types;

// io
import helios.util.io.BasicStringFileReader;

// rendering
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.rendering.shader.UniformSemantics;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.rendering.model.Material;
import helios.rendering.model.config.MeshConfig;
import helios.rendering.model.config.MaterialProperties;
import helios.rendering.model.config.MaterialPropertiesOverride;
import helios.rendering.model.config.PrimitiveType;
import helios.rendering.RenderPassFactory;
import helios.rendering.RenderPrototype;
import helios.rendering.Renderable;
import helios.rendering.Viewport;
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
import helios.scene.CameraSceneNode;

using namespace helios::scene;
using namespace helios::input;
using namespace helios::input::types;
using namespace helios::ext::glfw::app;
using namespace helios::ext::glfw::window;

#define SCALING_FACTOR 1.0f/5.0f

void updateButton(
    helios::scene::SceneNode& button,
    const bool pressed,
    helios::rendering::model::config::MaterialPropertiesOverride& originalOverride) {

    button.scale(helios::math::vec3f(SCALING_FACTOR * (pressed ? 1.2f : 1.0f)));
    if (button.renderable()->hasMaterialOverride()) {
        const auto baseColor = *originalOverride.baseColor;
        button.renderable()->materialOverride()->baseColor = helios::math::vec4f(baseColor[0], baseColor[1], baseColor[2], pressed ? 1.0f : baseColor[3]);
    }
}


void updateDpad(helios::scene::SceneNode& dpadButton, const bool pressed) {

    dpadButton.scale(helios::math::vec3f(SCALING_FACTOR * (pressed ? 1.2f : 1.0f)));
    if (dpadButton.renderable()->hasMaterialOverride()) {
        const auto baseColor = *dpadButton.renderable()->materialOverride()->baseColor;
        dpadButton.renderable()->materialOverride()->baseColor = helios::math::vec4f(baseColor[0], baseColor[1], baseColor[2], pressed ? 1.0f : 0.5f);
    }
}

int main() {

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Game Controller Input Demo"
    );

    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = win->addViewport(std::make_shared<helios::rendering::Viewport>());
    mainViewport->setCamera(std::make_shared<Camera>());
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
    auto circleMaterialProps = std::make_shared<helios::rendering::model::config::MaterialProperties>(
        helios::math::vec4f(1.0f, 0.0f, 1.0f, 0.5f),
        0.0f
    );

    auto lineMaterialProps = std::make_shared<helios::rendering::model::config::MaterialProperties>(
        helios::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f),
        0.0f
    );

    // create circle RenderPrototype
    const auto circleShape      = helios::rendering::asset::shape::basic::Circle();
    const auto circleMeshConfig = std::make_shared<const helios::rendering::model::config::MeshConfig>(circleShape.primitiveType());
    const auto circleMesh       = std::make_shared<const helios::ext::opengl::rendering::model::OpenGLMesh>(circleShape, circleMeshConfig);
    const auto circleMaterial   = std::make_shared<const helios::rendering::model::Material>(shader, circleMaterialProps);
    const auto circlePrototype  = std::make_shared<const helios::rendering::RenderPrototype>(circleMaterial, circleMesh);

    // create line RenderPrototype
    const auto lineShape      = helios::rendering::asset::shape::basic::Line();
    const auto lineMeshConfig = std::make_shared<const helios::rendering::model::config::MeshConfig>(lineShape.primitiveType());
    const auto lineMesh       = std::make_shared<const helios::ext::opengl::rendering::model::OpenGLMesh>(lineShape, lineMeshConfig);
    const auto lineMaterial   = std::make_shared<const helios::rendering::model::Material>(shader, lineMaterialProps);
    const auto linePrototype  = std::make_shared<const helios::rendering::RenderPrototype>(lineMaterial, lineMesh);

    // create rectangle RenderPrototype
    const auto recShape       = helios::rendering::asset::shape::basic::Rectangle();
    const auto recShapeConfig = std::make_shared<const helios::rendering::model::config::MeshConfig>(recShape.primitiveType());
    const auto recMesh        = std::make_shared<const helios::ext::opengl::rendering::model::OpenGLMesh>(recShape, recShapeConfig);
    const auto recMaterial    = std::make_shared<const helios::rendering::model::Material>(shader, lineMaterialProps);
    const auto recPrototype   = std::make_shared<const helios::rendering::RenderPrototype>(recMaterial, recMesh);

    const auto circleRenderable = std::make_shared<helios::rendering::Renderable>(circlePrototype);
    const auto lineRenderable   = std::make_shared<helios::rendering::Renderable>(linePrototype);
    const auto recRenderable    = std::make_shared<helios::rendering::Renderable>(recPrototype);

    // create the button renderables
    auto buttonMaterialPropsOverride = helios::rendering::model::config::MaterialPropertiesOverride(
        helios::math::vec4f(0.25f, 0.96f, 0.35f, 0.5f),
        0.0f
    );

    auto buttonRenderableA = std::make_shared<helios::rendering::Renderable>(
        circlePrototype, buttonMaterialPropsOverride
    );
    auto buttonRenderableB = std::make_shared<helios::rendering::Renderable>(
        circlePrototype, buttonMaterialPropsOverride
    );
    auto buttonRenderableX = std::make_shared<helios::rendering::Renderable>(
        circlePrototype, buttonMaterialPropsOverride
    );
    auto buttonRenderableY = std::make_shared<helios::rendering::Renderable>(
        circlePrototype, buttonMaterialPropsOverride
    );

    //create the dpad renderables
    auto dpadRenderableLft = std::make_shared<helios::rendering::Renderable>(
    recPrototype, helios::rendering::model::config::MaterialPropertiesOverride(
        helios::math::vec4f(1.0f, 1.0f, 0.0f, 0.5f),
        0.0f
    )
    );
    auto dpadRenderableRgt = std::make_shared<helios::rendering::Renderable>(
    recPrototype, helios::rendering::model::config::MaterialPropertiesOverride(
        helios::math::vec4f(0.0f, 1.0f, 1.0f, 0.5f),
        0.0f
    )
    );
    auto dpadRenderableUp = std::make_shared<helios::rendering::Renderable>(
    recPrototype, helios::rendering::model::config::MaterialPropertiesOverride(
        helios::math::vec4f(1.0f, 0.0f, 1.0f, 0.5f),
        0.0f
    )
    );
    auto dpadRenderableDown = std::make_shared<helios::rendering::Renderable>(
    recPrototype, helios::rendering::model::config::MaterialPropertiesOverride(
        helios::math::vec4f(1.0f, 1.0f, 1.0f, 0.5f),
        0.0f
    )
    );
    // ------------------------------
    //  Scene
    // ------------------------------
    auto cullingStrategy = std::make_unique<helios::scene::CullNoneStrategy>();
    auto scene = helios::scene::Scene(std::move(cullingStrategy));

    // camera
    const auto camera_ptr = scene.addNode(std::make_unique<helios::scene::CameraSceneNode>(mainViewport->camera()));
    assert(camera_ptr != nullptr && "unexpected nullptr for circleNode");

    // scene nodes
    const auto stickLeftNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(circleRenderable));
    auto stickAxisLeft_ptr = stickLeftNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(lineRenderable));

    // right stick
    const auto stickRightNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(circleRenderable));
    auto stickAxisRight_ptr = stickRightNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(lineRenderable));

    // left/right trigger
    const auto triggerLeftNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(recRenderable));
    const auto triggerRightNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>(recRenderable));

    // buttons
    const auto buttonGroupNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>());
    const auto buttonA_ptr = buttonGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(buttonRenderableA));
    const auto buttonB_ptr = buttonGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(buttonRenderableB));
    const auto buttonX_ptr = buttonGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(buttonRenderableX));
    const auto buttonY_ptr = buttonGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(buttonRenderableY));

    // dpad

    const auto dpadGroupNode_ptr = scene.addNode(std::make_unique<helios::scene::SceneNode>());
    const auto dpadLft_ptr = dpadGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(dpadRenderableLft));
    const auto dpadRgt_ptr = dpadGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(dpadRenderableRgt));
    const auto dpadUp_ptr = dpadGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(dpadRenderableUp));
    const auto dpadDown_ptr = dpadGroupNode_ptr->addChild(std::make_unique<helios::scene::SceneNode>(dpadRenderableDown));



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

    // ------------------------------
    //  Positioning and resizing
    // ------------------------------
    float basePosX          = 0.40f;
    float basePosY          = 0.40f;

    stickLeftNode_ptr->scale(helios::math::vec3f(SCALING_FACTOR, SCALING_FACTOR, 0.0f));
    stickLeftNode_ptr->translate(helios::math::vec3f(-basePosX, 0.0f, 0.0f));
    stickRightNode_ptr->scale(helios::math::vec3f(SCALING_FACTOR, SCALING_FACTOR, 0.0f));
    stickRightNode_ptr->translate(helios::math::vec3f(basePosX, 0.0f, 0.0f));

    const auto triggerBaseScale       = helios::math::vec3f(0.20, 0.25f * SCALING_FACTOR, 0.0f);
    auto triggerAabbWidth        = 2*triggerBaseScale[0];
    const auto triggerLftBasePosition = helios::math::vec3f((-basePosX + SCALING_FACTOR), basePosY, 0.0f);
    const auto triggerRgtBasePosition = helios::math::vec3f((basePosX - SCALING_FACTOR), basePosY, 0.0f);

    buttonGroupNode_ptr->scale(helios::math::vec3f(SCALING_FACTOR));
    buttonGroupNode_ptr->translate(helios::math::vec3f(basePosX, -basePosY, 0.0f));
    buttonA_ptr->translate(helios::math::vec3f(0.0f, -0.5f, 0.0f));
    buttonB_ptr->translate(helios::math::vec3f(0.5f, 0.0f, 0.0f));
    buttonX_ptr->translate(helios::math::vec3f(-0.5f, 0.0f, 0.0f));
    buttonY_ptr->translate(helios::math::vec3f(0.0f, 0.5f, 0.0f));

    dpadGroupNode_ptr->scale(helios::math::vec3f(SCALING_FACTOR));
    dpadUp_ptr->scale(helios::math::vec3f(SCALING_FACTOR));
    dpadLft_ptr->scale(helios::math::vec3f(SCALING_FACTOR));
    dpadDown_ptr->scale(helios::math::vec3f(SCALING_FACTOR));
    dpadRgt_ptr->scale(helios::math::vec3f(SCALING_FACTOR));

    dpadGroupNode_ptr->translate(helios::math::vec3f(-basePosX, -basePosY, 0.0f));
    dpadUp_ptr->translate(helios::math::vec3f(0.0f, 0.5f, 0.0f));
    dpadLft_ptr->translate(helios::math::vec3f(-0.5f, 0.0f, 0.0f));
    dpadDown_ptr->translate(helios::math::vec3f(0.0f, -0.5f, 0.0f));
    dpadRgt_ptr->translate(helios::math::vec3f(0.5f, 0.0f, 0.0f));


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
        stickAxisLeft_ptr->scale(helios::math::vec3f(gamepadState.left()*0.5f));
        stickAxisLeft_ptr->translate(helios::math::vec3f(gamepadState.left()*0.5f));

        stickAxisRight_ptr->scale(helios::math::vec3f(gamepadState.right()*0.5f));
        stickAxisRight_ptr->translate(helios::math::vec3f(gamepadState.right()*0.5f));

        triggerLeftNode_ptr->scale(helios::math::vec3f(triggerBaseScale[0]*leftTrigger, triggerBaseScale[1],  0.0f));
        triggerLeftNode_ptr->translate(triggerLftBasePosition + (helios::math::vec3f(-(triggerAabbWidth/2.0f)*leftTrigger, 0.0f, 0.0f)));

        triggerRightNode_ptr->scale(helios::math::vec3f(triggerBaseScale[0]*rightTrigger, triggerBaseScale[1],  0.0f));
        triggerRightNode_ptr->translate(triggerRgtBasePosition + (helios::math::vec3f((triggerAabbWidth/2.0f)*rightTrigger, 0.0f, 0.0f)));

        // buttons activation
        updateButton(*buttonA_ptr, gamepadState.buttonA(), buttonMaterialPropsOverride);
        updateButton(*buttonB_ptr, gamepadState.buttonB(), buttonMaterialPropsOverride);
        updateButton(*buttonX_ptr, gamepadState.buttonX(), buttonMaterialPropsOverride);
        updateButton(*buttonY_ptr, gamepadState.buttonY(), buttonMaterialPropsOverride);

        // dpad activation
        updateDpad(*dpadUp_ptr, gamepadState.buttonDpadUp());
        updateDpad(*dpadDown_ptr, gamepadState.buttonDpadDown());
        updateDpad(*dpadRgt_ptr, gamepadState.buttonDpadRight());
        updateDpad(*dpadLft_ptr, gamepadState.buttonDpadLeft());


        auto snapshot = scene.createSnapshot(mainViewport);
        if (snapshot.has_value()) {
            auto renderPass = helios::rendering::RenderPassFactory::getInstance().buildRenderPass(*snapshot);
            app->renderingDevice().render(renderPass);
        }

        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}