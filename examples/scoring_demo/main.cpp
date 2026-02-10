// ============================================================================
// Includes
// ============================================================================
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <glad/gl.h>
#include <iostream>
#include <numbers>


// ============================================================================
// Module Imports
// ============================================================================

import helios;
import helios.ext;
import helios.examples.scoring;


// ============================================================================
// Using Declarations
// ============================================================================

using namespace helios::ext::glfw::app;
using namespace helios::rendering;
using namespace helios::rendering::mesh;
using namespace helios::rendering::material;
using namespace helios::rendering::shader;
using namespace helios::ext::opengl::rendering;
using namespace helios::ext::opengl::rendering::shader;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::rendering::asset::shape::basic;
using namespace helios::input;
using namespace helios::input::gamepad;
using namespace helios::input::types;
using namespace helios::ext::glfw::window;
using namespace helios::util::io;
using namespace helios::scene;
using namespace helios::math;


// ============================================================================
// Entry Point
// ============================================================================

int main() {
    // ========================================
    // Constants
    // ========================================
    constexpr float SCREEN_WIDTH  = 1980;
    constexpr float SCREEN_HEIGHT = 1080;

    constexpr float CELL_LENGTH        = 5.0f;
    constexpr float SPACESHIP_LENGTH   = 5.0f;
    constexpr float GRID_X             = 29.0f;
    constexpr float GRID_Y             = 19.0f;
    constexpr float FOVY               = helios::math::radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;

    constexpr helios::math::vec3f GRID_SCALE{GRID_X * CELL_LENGTH, GRID_Y * CELL_LENGTH, 0.0f};

    constexpr auto TOP_LEFT_COORDINATES = helios::math::vec3f(GRID_SCALE[0] * - 0.5f, GRID_SCALE[1] * 0.5f, 0.0f);
    constexpr auto TOP_RIGHT_COORDINATES = helios::math::vec3f(GRID_SCALE[0] *  0.5f, GRID_SCALE[1] * 0.5f, 0.0f);
    constexpr auto BOTTOM_LEFT_COORDINATES =  helios::math::vec3f(GRID_SCALE[0] * - 0.5f, GRID_SCALE[1] * -0.5f, 0.0f);
    constexpr auto BOTTOM_RIGHT_COORDINATES =  helios::math::vec3f(GRID_SCALE[0] * 0.5f, GRID_SCALE[1] * -0.5f, 0.0f);

    constexpr auto BLUE_ENEMY_SCALE = helios::math::vec3f{SPACESHIP_LENGTH, SPACESHIP_LENGTH/2.0f, 0.0f};

    constexpr auto SPACESHIP_SPAWN_POSITION = helios::math::vec3f(0.0f, 0.0f, 0.0f);
    constexpr auto SPACESHIP_SIZE  = helios::math::vec3f(SPACESHIP_LENGTH, SPACESHIP_LENGTH, 0.0f);
    
    // ========================================
    // 1. Application and Window Setup
    // ========================================

    // bootstrap the components
    helios::engine::bootstrap::registerAllComponents();

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Scoring Demo", SCREEN_WIDTH, SCREEN_HEIGHT, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

    mainViewport->setClearFlags(std::to_underlying(ClearFlags::Color))
                  .setClearColor(vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->addViewport(mainViewport);

    // Get the InputManager for handling keyboard input
    helios::input::InputManager& inputManager = app->inputManager();
    // register the gamepads
    unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);

    const auto basicStringFileReader = BasicStringFileReader();

    // ui related config
    auto hudScene = std::make_unique<helios::scene::Scene>(
       std::make_unique<helios::scene::CullNoneStrategy>()
   );

    // Create a viewport for UI rendering.
    auto hudViewport = std::make_shared<helios::rendering::Viewport>(
        // move the viewport to the upper right
        0.0f, 0.0f, 1.0f, 1.0f, helios::engine::core::data::ViewportId{"hudViewport"}
    );

    hudViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Depth));


    // Configure orthographic camera for 2D text rendering.
    // Origin (0,0) is at the bottom-left corner of the screen.
    auto hudCamera = std::make_unique<helios::scene::Camera>();
    auto hudCameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(hudCamera));
    auto* hudCameraSceneNode_ptr = hudCameraSceneNode.get();

    hudCameraSceneNode_ptr->setInheritance(helios::math::TransformType::Translation);
    hudViewport->setCameraSceneNode(hudCameraSceneNode_ptr);
    hudCameraSceneNode_ptr->setTranslation(helios::math::vec3f(0.0f, 0.0f, -100.0f));
    hudCameraSceneNode_ptr->camera().setOrtho(
        0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    hudCameraSceneNode_ptr->lookAtLocal(
        helios::math::vec3f(0.0f, 0.0f, 0.0f),
        helios::math::vec3f(0.0f, 1.0f, 0.0f)
    );

    // add camera first, then add the viewport - the camera needs the render target's size,
    // so the proper order is important.
    win->addViewport(hudViewport);
    std::ignore = hudScene->addNode(std::move(hudCameraSceneNode));

    // ========================================
    // 1.2. ImGui & Tooling Setup
    // ========================================
    // Get native GLFW window
    // GLFWwindow* nativeWindow = win->nativeHandle();
    auto imguiBackend = helios::ext::imgui::ImGuiGlfwOpenGLBackend(win->nativeHandle());
    auto imguiOverlay = helios::ext::imgui::ImGuiOverlay::forBackend(&imguiBackend);
    auto fpsMetrics = helios::engine::tooling::FpsMetrics();
    auto stopwatch = std::make_unique<helios::util::time::Stopwatch>();
    auto framePacer = helios::engine::tooling::FramePacer(std::move(stopwatch));
    // set target framerate
    framePacer.setTargetFps(0.0f);
    helios::engine::tooling::FrameStats frameStats{};
    auto menu = new helios::ext::imgui::widgets::MainMenuWidget();
    auto fpsWidget = new helios::ext::imgui::widgets::FpsWidget(&fpsMetrics, &framePacer);
    auto gamepadWidget = new helios::ext::imgui::widgets::GamepadWidget(&inputManager);
    auto logWidget = new helios::ext::imgui::widgets::LogWidget();
    auto cameraWidget = new helios::ext::imgui::widgets::CameraWidget();
    imguiOverlay.addWidget(menu);
    imguiOverlay.addWidget(fpsWidget);
    imguiOverlay.addWidget(gamepadWidget);
    imguiOverlay.addWidget(logWidget);
    imguiOverlay.addWidget(cameraWidget);

    // ========================================
    // 1.3 Logger Configuration
    // ========================================
    helios::util::log::LogManager::getInstance().enableLogging(true);
    auto imguiLogSink = std::make_shared<helios::ext::imgui::ImGuiLogSink>(logWidget);
    helios::util::log::LogManager::getInstance().enableSink(imguiLogSink);

    // ========================================
    // 2. Shader Creation
    // ========================================
    auto defaultShader =
            std::make_shared<OpenGLShader>(
    "./resources/cube.vert",
    "./resources/cube.frag", basicStringFileReader);

    // Map the ModelMatrix uniform to location 1 in the shader
    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    bool mapping = uniformLocationMap->set(UniformSemantics::ModelMatrix, 1);
    mapping = uniformLocationMap->set(UniformSemantics::ViewMatrix, 2);
    mapping = uniformLocationMap->set(UniformSemantics::ProjectionMatrix, 3);
    mapping = uniformLocationMap->set(UniformSemantics::MaterialBaseColor, 4);

    defaultShader->setUniformLocationMap(std::move(uniformLocationMap));

    // ========================================================================
    // 2.1. Text Shader Setup
    // ========================================================================
    // Configure uniform locations for the text shader.
    auto glyphUniformLocationMap = std::make_unique<
        helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap
    >();
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, 1);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextTexture, 3);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextColor, 4);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ModelMatrix, 8);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ViewMatrix, 9);

    // Create the text shader from vertex and fragment shader files.
    auto glyphShader = std::make_shared<helios::ext::opengl::rendering::shader::OpenGLShader>(
        "resources/font_shader.vert",
        "resources/font_shader.frag",
        helios::util::io::BasicStringFileReader()
    );
    glyphShader->setUniformLocationMap(std::move(glyphUniformLocationMap));

    // ========================================
    // 3. Scene Graph Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<helios::scene::Scene>(std::move(frustumCullingStrategy));


    // ========================================
    // 4. Camera Setup
    // ========================================
    auto mainViewportCam = std::make_unique<helios::scene::Camera>();
    auto cameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(mainViewportCam));
    auto cameraSceneNode_ptr = cameraSceneNode.get();

    cameraSceneNode_ptr->setInheritance(
        helios::math::TransformType::Translation
    );
    mainViewport->setCameraSceneNode(cameraSceneNode_ptr);
    cameraSceneNode_ptr->setTranslation(
        helios::math::vec3f(0.0f, 0.0f, -100.0f)//-(GRID_Y*CELL_LENGTH / 2.0f)/tan(FOVY/2))
    );
    cameraSceneNode_ptr->camera().setPerspective(
        FOVY,
        ASPECT_RATIO_NUMER / ASPECT_RATIO_DENOM,
        0.1f,
        1000.0f
    );
    cameraSceneNode_ptr->lookAtLocal(
        vec3f(0.0f, 0.0f, 0.0f),
        vec3f(0.0f, 1.0f, 0.0f)
    );
    cameraWidget->addCameraSceneNode("Main Camera", cameraSceneNode_ptr);

    // ========================================
    // 5. GameWorld and Level Setup
    // ========================================

    helios::engine::runtime::gameloop::GameLoop gameLoop{};
    helios::engine::runtime::world::GameWorld gameWorld{};

    auto level = std::make_unique<helios::engine::runtime::world::Level>(&(scene.get()->root()));
    auto* levelPtr = level.get();
    level->setBounds(
        helios::math::aabb{
     -(GRID_X * CELL_LENGTH)/2.0f, -(GRID_Y * CELL_LENGTH)/2.0f, 0.0f,
            (GRID_X * CELL_LENGTH)/2.0f, (GRID_Y * CELL_LENGTH)/2.0f, 0.0f
        },
        helios::core::units::Unit::Meter
    );
    gameWorld.setLevel(std::move(level));

    // ========================================================================
    // 5.5 Font Loading
    // ========================================================================
    // Load the font via the rendering device's font resource provider.
    auto fontIdRoboto = helios::engine::core::data::FontId{"roboto"};
    auto& fontResourceProvider = app->renderingDevice().fontResourceProvider();
    fontResourceProvider.loadFont(fontIdRoboto, 24, "resources/Roboto-SemiBoldItalic.ttf");

    // ========================================================================
    // 5.6. Text Prototype and Renderable Creation
    // ========================================================================
    // Create a reusable text prototype with shader and font configuration.
    auto uiTextPrototype = std::make_shared<helios::rendering::text::TextRenderPrototype>(
        glyphShader,
        std::make_shared<helios::rendering::text::TextShaderProperties>(),
        &fontResourceProvider
    );


    // ========================================
    // 6. GameObject Prefabs
    // ========================================

    auto scoreText = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&](auto& rnb) {
            rnb.textRenderable()
               .text("0")
               .fontId(fontIdRoboto)
               .fontScale(1.0f)
               .fontResourceProvider(app->renderingDevice().fontResourceProvider())
               .shader(glyphShader)
               .color(helios::util::Colors::WhiteSmoke)
               .asUiText("{:.0f}")
               .attachTo(&hudScene->root());

        })
        .withObserver([&](auto& ob) {
           ob.observe()
             .scorePool(helios::engine::core::data::ScorePoolId{"playerOneScorePool"});
       })
        .withUiTransform([](auto& tb) {
            tb.transform()
              .pivot(helios::engine::modules::ui::layout::Anchor::TopRight)
              .viewport(helios::engine::core::data::ViewportId{"hudViewport"})
              .anchor(helios::engine::modules::ui::layout::Anchor::TopRight)
              .margins({60.0f, 50.0f, 0.0f, 0.0f});

        })
        .make();

    auto highScoreText = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&](auto& rnb) {
            rnb.textRenderable()
               .text("Highscore: 00000000")
               .fontId(fontIdRoboto)
               .fontScale(0.6f)
               .fontResourceProvider(app->renderingDevice().fontResourceProvider())
               .shader(glyphShader)
               .color(helios::util::Colors::LightGray)
               .attachTo(&hudScene->root());
        })
        .withUiTransform([](auto& tb) {
            tb.transform()
              .pivot(helios::engine::modules::ui::layout::Anchor::TopRight)
              .viewport(helios::engine::core::data::ViewportId{"hudViewport"})
              .anchor(helios::engine::modules::ui::layout::Anchor::TopRight)
              .margins({25.0f, 50.0f, 0.0f, 0.0f});
        })
        .make();

    // projectile game object
    auto projectilePrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Ellipse>(0.5f, 0.2f, 8))
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(2.2f, 0.8f, 0.0f), helios::core::units::Unit::Meter);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::scoring::CollisionId::Projectile)
              .useBoundingBox()
              .hitPolicy(helios::engine::modules::physics::collision::types::HitPolicy::OneHit)
              .reportCollisions(true)
              .solidCollisionMask(helios::examples::scoring::CollisionId::Enemy)
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Enemy,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn |
                  helios::engine::modules::physics::collision::types::CollisionBehavior::PassEvent
              )
              .dealDamage(100.0f, helios::examples::scoring::CollisionId::Enemy);

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn);
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(80.0f)
               .instantAcceleration(true);
            mcb.steering()
               .steeringSetsDirection(false)
               .instantSteering(true);
        })
        .withSpawn([](auto& sb) {
             sb.spawn()
               .useSpawnProfile()
               .trackEmitter();
         })
        .make();

    // THE GRID
    auto theGrid = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Turquoise.withW(0.2f))
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Grid>(29, 19))
               .attachTo(&root);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .useBoundingBox();
        })
        .withTransform([GRID_SCALE](auto& tb) {
            tb.transform()
              .translate(helios::math::vec3f(0.0f, 0.0f, 0.5f))
              .scale(GRID_SCALE);
        })
        .make();


    
    // ship game object
    auto shipGameObject = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
               .attachTo(&root);
        })
        .withTransform([&](auto& tb) {
            tb.transform()
              .scale(SPACESHIP_SIZE)
              .translate(SPACESHIP_SPAWN_POSITION);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::scoring::CollisionId::Player)
              .useBoundingBox()
              .hitPolicy(helios::engine::modules::physics::collision::types::HitPolicy::OneHit)
              .reportCollisions(true)
              .solidCollisionMask(helios::examples::scoring::CollisionId::Enemy)
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Enemy,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Bounce);
        })
        .withCombat([](auto& ccb) {
            ccb.weapon()
               .fireRate(5.0f);//15.0f);//5.0f;
        })
        .withScoring([](auto& sb) {
            sb.scorePool()
              .poolId(helios::engine::core::data::ScorePoolId{"playerOneScorePool"});
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(30.0f)
               .instantAcceleration(false);
            mcb.steering()
               .steeringSetsDirection(true)
               .instantSteering(false);
        })
        .withSpawn([](auto& sb) {
            sb.spawn()
              .useSpawnProfile();
        })
        .make();

    // GIZMO Left stick
    auto leftStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::White)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        }).make();

    auto rightStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        })
        .make();

    auto shipDirectionGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Red)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        })
        .make();


    // purple enemy
    auto purpleEnemyPrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::LimeGreen)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Rectangle>())
               .attachTo(&root);
        })
        .withHealth([](auto& hsb) {
            hsb.health()
               .maxHealth(100.0f);
        })
        .withScoring([](helios::engine::builder::gameObject::builders::ScoringBuilder& sb) {
            sb.scoreValue()
              .score<helios::engine::mechanics::scoring::types::KillReward>(50);
        })
        .withCombat([](auto& cb) {
            cb.combat()
              .trackLastAttacker();
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_LENGTH/2.0f, SPACESHIP_LENGTH/2.0f, 0.0f));
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::scoring::CollisionId::Enemy)
              .useBoundingBox()
              .reportCollisions(false)
              .solidCollisionMask(
                helios::examples::scoring::CollisionId::Player | helios::examples::scoring::CollisionId::Projectile)
              /*.onSolidCollision(
                  helios::examples::scoring::CollisionId::Player,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              )
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Projectile,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::PassEvent
              )*/;

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Reflect);
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(5.0f)
               .instantAcceleration(true);
        })
        .withEffects([](auto& eb) {
            eb.gfx()
              .spin(270.0f, helios::math::Z_AXISf);
        })
        .withSpawn([](auto& sb) {
            sb.spawn()
              .useSpawnProfile();
        })
        .make();


    // orange enemy
    auto orangeEnememyPrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Orange)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_LENGTH/2.0f, SPACESHIP_LENGTH/2.0f, 0.0f));
        })
        .withHealth([](auto& hsb) {
            hsb.health()
               .maxHealth(100.0f);
        })
        .withScoring([](helios::engine::builder::gameObject::builders::ScoringBuilder& sb) {
            sb.scoreValue()
              .score<helios::engine::mechanics::scoring::types::KillReward>(100);
        })
        .withCombat([](auto& cb) {
            cb.combat()
              .trackLastAttacker();
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::scoring::CollisionId::Enemy)
              .useBoundingBox()
              .reportCollisions(false)
              .solidCollisionMask(
                helios::examples::scoring::CollisionId::Player | helios::examples::scoring::CollisionId::Projectile)
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Player,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              )
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Projectile,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

            cb.levelBoundsCollision()
              .restitution(0.0001f)
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Bounce)
              .respondWith(helios::engine::modules::physics::collision::types::CollisionResponse::AlignHeadingToDirection);
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(25.0f)
               .acceleration(25.0f);

            mcb.steering()
               .steeringSetsDirection(false)
               .instantSteering(false);
        })
        .withSpawn([](auto& sb) {
            sb.spawn()
              .useSpawnProfile();
        })
        .make();

    // blue enemy
    auto blueEnemyPrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::LightBlue)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Rhombus>())
               .attachTo(&root);
        })
        .withTransform([BLUE_ENEMY_SCALE](auto& tb) {
            tb.transform()
              .scale(BLUE_ENEMY_SCALE);
        })
        .withHealth([](auto& hsb) {
            hsb.health()
               .maxHealth(100.0f);
        })
        .withScoring([](helios::engine::builder::gameObject::builders::ScoringBuilder& sb) {
            sb.scoreValue()
              .score<helios::engine::mechanics::scoring::types::KillReward>(150);
        })
        .withCombat([](auto& cb) {
            cb.combat()
              .trackLastAttacker();
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::scoring::CollisionId::Enemy)
              .useBoundingBox()
              .reportCollisions(false)
              .solidCollisionMask(
                helios::examples::scoring::CollisionId::Player | helios::examples::scoring::CollisionId::Projectile)
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Player,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              )
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Projectile,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Reflect)
              .respondWith(helios::engine::modules::physics::collision::types::CollisionResponse::AlignHeadingToDirection);
        })
        .withMotion([&](auto& mcb) {
           mcb.move2D()
              .speed( shipGameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>()->movementSpeed()  * 0.5f)
              .instantAcceleration(true);

           mcb.steering()
              .steeringSetsDirection(true)
              .instantSteering(true);
        })
        .withSpawn([](auto& sb) {
            sb.spawn()
              .useSpawnProfile();
        })
        .withLifecycle([](auto& lcb) {
            lcb.lifecycle()
               .useDelayedComponentEnabler();
        })
        .withAi([&](auto& aib) {
            aib.chasing()
               .target(shipGameObject.entityHandle())
               .seekInterval(0.05f);
        })
        .make();


    using namespace helios::engine::runtime::spawn;

    // ========================================
    // 7. Spawn System Configuration
    // ========================================
    auto spawnSchedulers = std::vector<std::unique_ptr<helios::engine::runtime::spawn::scheduling::SpawnScheduler>>();

    constexpr helios::engine::core::data::GameObjectPoolId PurpleEnemyPoolId{"purple_pool"};
    constexpr helios::engine::core::data::GameObjectPoolId OrangeEnemyPoolId{"orange_pool"};
    constexpr helios::engine::core::data::GameObjectPoolId BlueEnemyPoolId{"blue_pool"};
    constexpr helios::engine::core::data::GameObjectPoolId ProjectilePoolId{"projectile_pool"};
    
    // spawn profile ids
    helios::engine::core::data::SpawnProfileId ProjectileSpawnSpawnProfileId{"projectile_spawn"};
    helios::engine::core::data::SpawnProfileId RandomSpawnSpawnProfileId{"random_spawn"};

    // spawn rule ids
    helios::engine::core::data::SpawnRuleId PurpleEnemySpawnConditionId{"purple_spawn_rule"};



    // register the managers.
    auto& poolManager      = gameWorld.addManager<helios::engine::runtime::pooling::GameObjectPoolManager>();
    auto& scorePoolManager = gameWorld.addManager<helios::engine::mechanics::scoring::ScorePoolManager>();
    auto& spawnManager     = gameWorld.addManager<helios::engine::runtime::spawn::SpawnManager>();

    scorePoolManager.addScorePool(helios::engine::core::data::ScorePoolId{"playerOneScorePool"});

    // tell the PoolManager which GameObjectPools to manage
    poolManager.addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                ProjectilePoolId,
                projectilePrefab,
                50
            )).addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                PurpleEnemyPoolId,
                purpleEnemyPrefab,
                50
                )).addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                BlueEnemyPoolId,
                blueEnemyPrefab,
                100
            )).addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                OrangeEnemyPoolId,
                orangeEnememyPrefab,
                static_cast<size_t>((GRID_X * CELL_LENGTH) / SPACESHIP_LENGTH)
            ));

    // create spawn profiles
    auto randomSpawnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        PurpleEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::RandomSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
        helios::engine::runtime::spawn::behavior::initializers::DirectionType::Random
        )
    );

    // +------------------------------------------------
    // | Spawn Profiles and Scheduling for Blue Enemy
    // +------------------------------------------------
    auto blueEnemySpawnScheduler = std::make_unique<helios::engine::runtime::spawn::scheduling::DefaultSpawnScheduler>();
    auto blueEnemyMassScheduler = std::make_unique<helios::engine::runtime::spawn::scheduling::DefaultSpawnScheduler>();

    // random spawning
    helios::engine::core::data::SpawnProfileId BlueRandomSpawnProfileId{"blue_spawn1"};
    helios::engine::core::data::SpawnRuleId BlueRandomSpawnConditionId{"blue_spawn_rule_1"};
    auto blueRandomSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        BlueEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::RandomSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
        helios::engine::runtime::spawn::behavior::initializers::DirectionType::Random
        )
    );
    auto blueRandomSpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(5.0f),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(1),
        BlueRandomSpawnConditionId
    );


    // mass spawn
    helios::engine::core::data::SpawnProfileId BlueMassSpawnProfileId{"blue_spawn_profile1"};
    helios::engine::core::data::SpawnRuleId BlueMassSpawnConditionId{"blue_mass_spawn1"};
    auto blueMassSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        BlueEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::DistributedSpawnPlacer<4>>(
            //make sure we do not spawn in the wall, otherwise the collision system is triggered
            TOP_LEFT_COORDINATES + helios::math::vec3f(CELL_LENGTH, -CELL_LENGTH, 0.0f),
            TOP_RIGHT_COORDINATES + helios::math::vec3f(-CELL_LENGTH, -CELL_LENGTH, 0.0f),
            BOTTOM_LEFT_COORDINATES + helios::math::vec3f(CELL_LENGTH, CELL_LENGTH, 0.0f),
            BOTTOM_RIGHT_COORDINATES + helios::math::vec3f(-CELL_LENGTH, CELL_LENGTH, 0.0f)
        ),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::InitializerList<2>>(
            std::make_unique<helios::engine::runtime::spawn::behavior::initializers::DelayedComponentEnablerInitializer<
                helios::engine::modules::physics::motion::components::Move2DComponent
            >>(0.5f, 25),
            std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
                helios::math::vec3f{0.0f}, helios::engine::runtime::spawn::behavior::initializers::DirectionType::Point
            )
        )
    );
    auto blueMassSpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::SpawnConditionAll>(
            std::make_unique<helios::engine::runtime::spawn::policy::conditions::RequestedAmountIsAvailableCondition>(),
            std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(30.0f)
        ),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(100),
        BlueMassSpawnConditionId
    );

   // spawnManager.addSpawnProfile(BlueRandomSpawnProfileId, std::move(blueRandomSpawnProfile));
    spawnManager.addSpawnProfile(BlueMassSpawnProfileId, std::move(blueMassSpawnProfile));

    //blueEnemySpawnScheduler->addRule(BlueRandomSpawnProfileId, std::move(blueRandomSpawnRule));
    blueEnemyMassScheduler->addRule(BlueMassSpawnProfileId, std::move(blueMassSpawnRule));
  //  blueEnemyMassScheduler->addSpawnCommandModifier(std::make_unique<helios::engine::runtime::spawn::DeferSpawn>(1.0f));


    spawnSchedulers.push_back(std::move(blueEnemyMassScheduler));
    // +-------------------------------------
    // | ^^ EO blue enemy spawn config
    // +-------------------------------------


    // +------------------------------------------------
    // | Spawn Profiles and Scheduling for Orange Enemy
    // +------------------------------------------------

    helios::engine::core::data::SpawnProfileId LeftColumnSpawnProfileId{"orange_left_spawn1"};
    auto leftColumnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        OrangeEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::AxisSpawnPlacer>(
            helios::math::vec3f(0.0f, -1.0f, 0.0f).normalize(),
            TOP_LEFT_COORDINATES
        ),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
            helios::math::X_AXISf
        )
    );
    helios::engine::core::data::SpawnProfileId RightColumnSpawnProfileId{"orange_right_spawn_1"};
    auto rightColumnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        OrangeEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::AxisSpawnPlacer>(
            helios::math::vec3f(0.0f, -1.0f, 0.0f).normalize(),
            TOP_RIGHT_COORDINATES
        ),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
            helios::math::X_AXISf * -1.0f
        )
    );

    helios::engine::core::data::SpawnProfileId TopRowSpawnProfileId{"orange_top_spawn_1"};
    auto topRowSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        OrangeEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::AxisSpawnPlacer>(
            helios::math::vec3f(1.0f, 0.0f, 0.0f).normalize(),
             TOP_LEFT_COORDINATES
        ),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
            helios::math::Y_AXISf * -1.0f
        )
    );
    helios::engine::core::data::SpawnProfileId BottomRowSpawnProfileId{"orange_bottom_spawn_1"};
    auto bottomRowSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        OrangeEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::AxisSpawnPlacer>(
            helios::math::vec3f(1.0f, 0.0f, 0.0f).normalize(),
            BOTTOM_LEFT_COORDINATES
        ),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
            helios::math::Y_AXISf
        )
    );

    helios::engine::core::data::SpawnRuleId OrangeEnemySpawnConditionIdLeftColumn{"orange_spawn_rule_1"};
    helios::engine::core::data::SpawnRuleId OrangeEnemySpawnConditionIdRightColumn{"orange_spawn_rule_2"};
    helios::engine::core::data::SpawnRuleId OrangeEnemySpawnConditionIdTopRow{"orange_spawn_rule_3"};
    helios::engine::core::data::SpawnRuleId OrangeEnemySpawnConditionIdBottomRow{"orange_spawn_ruke_4"};

    constexpr size_t OBJECT_AMOUNT_X = GRID_X * CELL_LENGTH / SPACESHIP_LENGTH;
    constexpr size_t OBJECT_AMOUNT_Y = GRID_Y * CELL_LENGTH / SPACESHIP_LENGTH;

    auto leftColumnOrangeEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
         std::make_unique<helios::engine::runtime::spawn::policy::SpawnConditionAll>(
         std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(15.0f),
         std::make_unique<helios::engine::runtime::spawn::policy::conditions::RequestedAmountIsAvailableCondition>()
         ),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(OBJECT_AMOUNT_Y),
        OrangeEnemySpawnConditionIdLeftColumn
    );

    auto rightColumnOrangeEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::SpawnConditionAll>(
            std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(15.0f),
            std::make_unique<helios::engine::runtime::spawn::policy::conditions::RequestedAmountIsAvailableCondition>()
        ),std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(OBJECT_AMOUNT_Y),

        OrangeEnemySpawnConditionIdRightColumn
    );

    auto topRowOrangeEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
    std::make_unique<helios::engine::runtime::spawn::policy::SpawnConditionAll>(
        std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(15.0f),
        std::make_unique<helios::engine::runtime::spawn::policy::conditions::RequestedAmountIsAvailableCondition>()
        ),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(OBJECT_AMOUNT_X),

        OrangeEnemySpawnConditionIdTopRow
    );

    auto bottomRowOrangeEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::SpawnConditionAll>(
    std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(15.0f),
    std::make_unique<helios::engine::runtime::spawn::policy::conditions::RequestedAmountIsAvailableCondition>()
    ),
    std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(OBJECT_AMOUNT_X),
        OrangeEnemySpawnConditionIdBottomRow
    );

    auto orangeEnemySpawnScheduler = std::make_unique<helios::engine::runtime::spawn::scheduling::CyclicSpawnScheduler<4>>();

    orangeEnemySpawnScheduler
        ->addRule(LeftColumnSpawnProfileId, std::move(leftColumnOrangeEnemySpawnRule))
         .addRule(TopRowSpawnProfileId, std::move(topRowOrangeEnemySpawnRule))
         .addRule(RightColumnSpawnProfileId, std::move(rightColumnOrangeEnemySpawnRule))
         .addRule(BottomRowSpawnProfileId, std::move(bottomRowOrangeEnemySpawnRule));

    spawnManager.addSpawnProfile(LeftColumnSpawnProfileId, std::move(leftColumnSpawnProfile));
    spawnManager.addSpawnProfile(RightColumnSpawnProfileId, std::move(rightColumnSpawnProfile));
    spawnManager.addSpawnProfile(TopRowSpawnProfileId, std::move(topRowSpawnProfile));
    spawnManager.addSpawnProfile(BottomRowSpawnProfileId, std::move(bottomRowSpawnProfile));

    spawnSchedulers.push_back(std::move(orangeEnemySpawnScheduler));

    // +-------------------------------------
    // | ^^ EO orange enemy spawn config
    // +-------------------------------------


    auto projectileSpawnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        ProjectilePoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::EmitterSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::EmitterInitializer>()
    );

    auto purpleEnemySpawnScheduler = std::make_unique<helios::engine::runtime::spawn::scheduling::DefaultSpawnScheduler>();
    auto purpleEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(5.0f),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(1),
        PurpleEnemySpawnConditionId
    );


    // let the SpawnManager know which SpawnProfiles to manage
    spawnManager.addSpawnProfile(RandomSpawnSpawnProfileId, std::move(randomSpawnSpawnProfile));

    spawnManager.addSpawnProfile(ProjectileSpawnSpawnProfileId, std::move(projectileSpawnSpawnProfile));
    
    // bind the spawn rules to the SpawnProfiles
    purpleEnemySpawnScheduler->addRule(RandomSpawnSpawnProfileId, std::move(purpleEnemySpawnRule));

    spawnSchedulers.push_back(std::move(purpleEnemySpawnScheduler));

    // ========================================
    // 8. Command Dispatchers
    // ========================================

    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::ScheduledSpawnPlanCommandDispatcher>()
    ).addDispatcher<helios::engine::runtime::spawn::commands::SpawnCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::SpawnCommandDispatcher>()
    ).addDispatcher<helios::engine::runtime::spawn::commands::DespawnCommand>(
     std::make_unique<helios::engine::runtime::spawn::dispatcher::DespawnCommandDispatcher>()
    ).addDispatcher<helios::engine::mechanics::scoring::commands::UpdateScoreCommand>(
        std::make_unique<helios::engine::mechanics::scoring::ScoreCommandDispatcher>()
    );

    // ========================================
    // 9. GameLoop Phase Configuration
    // ========================================
    //
    //                   +----------+
    //                   |  FRAME N |
    //                   +----------+
    //
    //           PRE    -    MAIN   -    POST
    //         UPDATE      RESOLVE      CLEANUP
    // +----------------------------------------------
    // | PRE PHASE: Input, Spawning, Movement
    // +----------------------------------------------

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Pre)
            .addPass()
            .addSystem<helios::engine::mechanics::scoring::systems::ScoreObserverSystem>()
            .addSystem<helios::engine::mechanics::input::systems::TwinStickInputSystem>(shipGameObject)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass()
            .addSystem<helios::engine::mechanics::spawn::systems::GameObjectSpawnSystem>(std::move(spawnSchedulers))
            .addSystem<helios::engine::mechanics::combat::systems::ProjectileSpawnSystem>(ProjectileSpawnSpawnProfileId)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass()
            .addSystem<helios::engine::modules::ai::systems::ChaseSystem>()
            .addSystem<helios::engine::modules::spatial::transform::systems::ScaleSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SteeringSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SpinSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::Move2DSystem>();

    // +----------------------------------------------
    // | MAIN PHASE: Collision Detection and Response
    // +----------------------------------------------
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Main)
            .addPass()
            .addSystem<helios::engine::modules::physics::collision::systems::BoundsUpdateSystem>()
            .addSystem<helios::engine::mechanics::bounds::systems::LevelBoundsBehaviorSystem>()
            .addSystem<helios::engine::modules::physics::collision::systems::GridCollisionDetectionSystem>(
                levelPtr->bounds(), CELL_LENGTH/2.0f
             )
            .addCommitPoint()
            .addPass()
            .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateResponseSystem>()
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::PassEvents)
            .addPass()
            .addSystem<helios::engine::mechanics::damage::systems::DamageOnCollisionSystem>()
            .addSystem<helios::engine::mechanics::health::systems::HealthUpdateSystem>();

    // +----------------------------------------------
    // | POST PHASE: Transform Composition and Cleanup
    // +----------------------------------------------
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Post)
             .addPass()
             .addSystem<helios::engine::modules::ui::binding::systems::Score2UiTextUpdateSystem>()
             .addSystem<helios::engine::mechanics::scoring::systems::CombatScoringSystem>()

             .addSystem<helios::engine::modules::ui::widgets::systems::UiTextBoundsUpdateSystem>()
             .addSystem<helios::engine::modules::ui::transform::systems::UiTransformSystem>()

             .addSystem<helios::engine::modules::spatial::transform::systems::ComposeTransformSystem>()
             .addSystem<helios::engine::modules::systems::scene::SceneSyncSystem>(scene.get())
             .addSystem<helios::engine::modules::spatial::transform::systems::TransformClearSystem>()
             .addSystem<helios::engine::mechanics::lifecycle::systems::DelayedComponentEnablerSystem>()
             .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateClearSystem>()
             .addSystem<helios::engine::mechanics::scoring::systems::ScoreObserverClearSystem>();


    float DELTA_TIME = 0.0f;



    // ========================================
    // 10. Main Game Loop
    // ========================================

    // embedd the immediate game objects
    leftStickGizmo.setActive(true);
    rightStickGizmo.setActive(true);
    shipDirectionGizmo.setActive(true);

    theGrid.setActive(true);
    std::ignore = scene->addNode(std::move(cameraSceneNode));

    //std::ignore = shipGameObject.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode()->addNode(std::move(cameraSceneNode));

    shipGameObject.setActive(true);
    auto* leftStickGizmoNode = leftStickGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* rightStickGizmoNode = rightStickGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* shipDirectionGizmoNode = shipDirectionGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();

    // add ui objects
    scoreText.setActive(true);
    highScoreText.setActive(true);



    // warm up world!
    // ENGINE INIT
    gameWorld.init();
    gameLoop.init(gameWorld);

    bool showImgui = false;
    bool tilde = false;

    while (!win->shouldClose()) {
        framePacer.beginFrame();

        // ----------------------------------------
        // 10.1 Event and Input Processing
        // ----------------------------------------
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

        // Check for ESC key to close the application
        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC] - Exiting..." << std::endl;
            win->setShouldClose(true);
        }

        if (!tilde && inputManager.isKeyPressed(Key::TILDE)) {
            tilde = true;
            showImgui = !showImgui;
        }
        if (tilde && inputManager.isKeyReleased(Key::TILDE)) {
            tilde = false;
        }

        // ----------------------------------------
        // 10.2 Game Logic Update
        // ----------------------------------------
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = helios::input::InputSnapshot(gamepadState);


        const auto viewportSnapshots = win->viewportSnapshots();
        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot, viewportSnapshots);


        // ----------------------------------------
        // 10.3 Gizmo / Debug Visualization Update
        // ----------------------------------------
        const auto* mcLft = shipGameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
        if (mcLft) {
            leftStickGizmoNode->setScale((mcLft->direction() * mcLft->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mcLft->velocity().normalize() * mcLft->speedRatio() * 4.0f);
        }
        const auto* mcRgt = shipGameObject.get<helios::engine::mechanics::combat::components::Aim2DComponent>();
        if (mcRgt) {
            rightStickGizmoNode->setScale((mcRgt->direction() * mcRgt->frequency()  * 4.0f).toVec3());
        }

        // ----------------------------------------
        // 10.4 Rendering
        // ----------------------------------------
        // render the game
        const auto& snapshot = scene->createSnapshot(*mainViewport);
        if (snapshot.has_value()) {
            auto renderPass = RenderPassFactory::getInstance().buildRenderPass(*snapshot);
            app->renderingDevice().render(renderPass);
        }

        // Render the UI scene.
        const auto& hudSnapshot = hudScene->createSnapshot(*hudViewport);
        if (hudSnapshot.has_value()) {
            auto uiRenderPass = helios::rendering::RenderPassFactory::getInstance()
                .buildRenderPass(*hudSnapshot);
            app->renderingDevice().render(uiRenderPass);
        }

        // ----------------------------------------
        // 10.5 ImGui Overlay Rendering
        // ----------------------------------------
        if (showImgui) {
            imguiOverlay.render();
        }

        // ----------------------------------------
        // 10.6 Frame Synchronization
        // ----------------------------------------
        // swap time / idle time should be read out here
        win->swapBuffers();

        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        DELTA_TIME = frameStats.totalFrameTime;
    }

    return EXIT_SUCCESS;
}
