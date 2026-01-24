// ============================================================================
// Includes
// ============================================================================
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <glad/gl.h>
#include <iostream>
#include <numbers>

#include "../../build/cmake_build_release/_deps/imgui-src/imgui.h"


// ============================================================================
// Module Imports
// ============================================================================

import helios;
import helios.ext;
import helios.examples.collisionDetection;


// ============================================================================
// Using Declarations
// ============================================================================

using namespace helios::ext::glfw::app;
using namespace helios::rendering;
using namespace helios::rendering::model;
using namespace helios::rendering::model::config;
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
    constexpr float CELL_SIZE          = 5.0f;
    constexpr float SPACESHIP_SIZE     = 5.0f;
    constexpr float GRID_X             = 29.0f;
    constexpr float GRID_Y             = 19.0f;
    constexpr float FOVY               = helios::math::radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;



    // ========================================
    // 1. Application and Window Setup
    // ========================================

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Collision Detection Demo", 1980, 1024, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
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

    // ========================================
    // 3. Game Object Creation (Rendering)
    // ========================================




    // ========================================
    // 6. Scene Graph Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<helios::scene::Scene>(std::move(frustumCullingStrategy));

    // Add the grid
  /*  auto* gridSceneNode = scene->addNode(std::make_unique<helios::scene::SceneNode>(std::move(gridRenderable)));

    auto* spaceshipSceneNode =
                    scene->addNode(std::make_unique<helios::scene::SceneNode>(std::move(spaceshipRenderable)));
*/
    /**
     * @todo we are using the aabb of the grid as the arenaBox for the projectile pool.
     * The grid has its zindex at 0 (min/max), this should be improved later
     */
  //  spaceshipSceneNode->setTranslation(helios::math::vec3f{0.0f, 0.0f, 0.0f});



    // ========================================
    // 7. Register mainViewport-Camera w/ Setup
    // ========================================
    auto mainViewportCam = std::make_unique<helios::scene::Camera>();
    auto cameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(mainViewportCam));
    auto cameraSceneNode_ptr = cameraSceneNode.get();

    cameraSceneNode_ptr->setInheritance(
        helios::math::TransformType::Translation
    );
    mainViewport->setCameraSceneNode(cameraSceneNode_ptr);
    cameraSceneNode_ptr->setTranslation(
        helios::math::vec3f(0.0f, 0.0f, -100.0f)//-(GRID_Y*CELL_SIZE / 2.0f)/tan(FOVY/2))
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
    // 9. Game-related Input-handling, GameWorld and GameObjects
    // ========================================

    helios::engine::runtime::gameloop::GameLoop gameLoop{};
    helios::engine::runtime::world::GameWorld gameWorld{};

    auto level = std::make_unique<helios::engine::runtime::world::Level>(&(scene.get()->root()));
    auto* levelPtr = level.get();
    level->setBounds(
        helios::math::aabb{
     -(GRID_X * CELL_SIZE)/2.0f, -(GRID_Y * CELL_SIZE)/2.0f, 0.0f,
            (GRID_X * CELL_SIZE)/2.0f, (GRID_Y * CELL_SIZE)/2.0f, 0.0f
        },
        helios::core::units::Unit::Meter
    );
    gameWorld.setLevel(std::move(level));

    
    // projectile game object
    auto projectilePrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::model::config::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Ellipse>(0.5f, 0.2f, 8))
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(2.2f, 0.8f, 0.0f), helios::core::units::Unit::Meter);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::collisionDetection::CollisionId::Projectile)
              .useBoundingBox()
              .reportCollisions(true)
              .solidCollisionMask(helios::examples::collisionDetection::CollisionId::Enemy)
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Enemy,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

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
               .useSpawnProfile();
         })
        .make();

    // THE GRID
    auto theGrid = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Turquoise.withW(0.2f))
               .primitiveType(helios::rendering::model::config::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Grid>(29, 19))
               .attachTo(&root);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .useBoundingBox();
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .translate(helios::math::vec3f(0.0f, 0.0f, 0.5f))
              .scale(helios::math::vec3f(GRID_X * CELL_SIZE, GRID_Y * CELL_SIZE, 0.0f));
        })
        .make();



    
    // ship game object
    auto shipGameObject = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::model::config::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_SIZE, SPACESHIP_SIZE, 0.0f));
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::collisionDetection::CollisionId::Player)
              .useBoundingBox()
              .reportCollisions(true)
              .solidCollisionMask(helios::examples::collisionDetection::CollisionId::Enemy)
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Enemy,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Bounce);
        })
        .withShooting([](auto& ccb) {

            ccb.weapon()
               .fireRate(5.0f);
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
        .gameObject()
        .withRendering([&defaultShader, &shipGameObject](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::White)
               .primitiveType(helios::rendering::model::config::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject.get())
               .inherit(helios::math::TransformType::Translation);
        }).make();

    auto rightStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &shipGameObject](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::model::config::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject.get())
               .inherit(helios::math::TransformType::Translation);
        })
        .make();

    auto shipDirectionGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &shipGameObject](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Red)
               .primitiveType(helios::rendering::model::config::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject.get())
               .inherit(helios::math::TransformType::Translation);
        })
        .make();


    // purple enemy
    auto purpleEnememyPrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::LimeGreen)
               .primitiveType(helios::rendering::model::config::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Rectangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_SIZE/2.0f, SPACESHIP_SIZE/2.0f, 0.0f));
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::collisionDetection::CollisionId::Enemy)
              .useBoundingBox()
              .reportCollisions(false)
              .solidCollisionMask(
                helios::examples::collisionDetection::CollisionId::Player | helios::examples::collisionDetection::CollisionId::Projectile)
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Player,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              )
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Projectile,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

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
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Orange)
               .primitiveType(helios::rendering::model::config::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_SIZE/2.0f, SPACESHIP_SIZE/2.0f, 0.0f));
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::collisionDetection::CollisionId::Enemy)
              .useBoundingBox()
              .reportCollisions(false)
              .solidCollisionMask(
                helios::examples::collisionDetection::CollisionId::Player | helios::examples::collisionDetection::CollisionId::Projectile)
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Player,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              )
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Projectile,
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
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.renderable()
               .shader(defaultShader)
               .color(helios::util::Colors::LightBlue)
               .primitiveType(helios::rendering::model::config::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Rhombus>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f{SPACESHIP_SIZE, SPACESHIP_SIZE/2.0f, 0.0f});
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::collisionDetection::CollisionId::Enemy)
              .useBoundingBox()
              .reportCollisions(false)
              .solidCollisionMask(
                helios::examples::collisionDetection::CollisionId::Player | helios::examples::collisionDetection::CollisionId::Projectile)
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Player,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              )
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Projectile,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Reflect)
              .respondWith(helios::engine::modules::physics::collision::types::CollisionResponse::AlignHeadingToDirection);
        })
        .withMotion([&](auto& mcb) {
           mcb.move2D()
              .speed( shipGameObject->get<helios::engine::modules::physics::motion::components::Move2DComponent>()->movementSpeed() - 3.0f)
              .instantAcceleration(true);

           mcb.steering()
              .steeringSetsDirection(true)
              .instantSteering(true);
        })
        .withSpawn([](auto& sb) {
            sb.spawn()
              .useSpawnProfile();
        })
        .withAi([&](auto& aib) {
            aib.chasing()
               .target(shipGameObject->guid())
               .seekInterval(0.05f);
        })
        .make();



    using namespace helios::engine::runtime::spawn;

    // --------------------------
    //     SPAWN CONFIGURATION
    // --------------------------

    constexpr helios::engine::core::data::GameObjectPoolId PurpleEnemyPoolId{1};
    constexpr helios::engine::core::data::GameObjectPoolId OrangeEnemyPoolId{2};
    constexpr helios::engine::core::data::GameObjectPoolId BlueEnemyPoolId{3};
    constexpr helios::engine::core::data::GameObjectPoolId ProjectilePoolId{4};
    
    // spawn profile ids
    helios::engine::core::data::SpawnProfileId ProjectileSpawnSpawnProfileId{1};
    helios::engine::core::data::SpawnProfileId RandomSpawnSpawnProfileId{2};
    helios::engine::core::data::SpawnProfileId VerticalSpawnSpawnProfileId{3};
    helios::engine::core::data::SpawnProfileId BlueSpawnSpawnProfileId{4};

    // spawn rule ids
    helios::engine::core::data::SpawnRuleId PurpleEnemySpawnConditionId{1};
    helios::engine::core::data::SpawnRuleId OrangeEnemySpawnConditionId{2};
    helios::engine::core::data::SpawnRuleId BlueEnemySpawnConditionId{3};


    // register the SpawnManager. This spawn manager will also be used as the SpawnCommandHandler for
    // SpawnCommands
    auto& poolManager = gameWorld.addManager<helios::engine::runtime::pooling::GameObjectPoolManager>();
    auto& spawnManager = gameWorld.addManager<helios::engine::runtime::spawn::SpawnManager>();


    // tell the PoolManager which GameObjectPools to manage
    poolManager.addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                ProjectilePoolId,
                std::move(projectilePrefab),
                50
            )).addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                PurpleEnemyPoolId,
                std::move(purpleEnememyPrefab),
                50
                )).addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                BlueEnemyPoolId,
                std::move(blueEnemyPrefab),
                50
            )).addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                OrangeEnemyPoolId,
                std::move(orangeEnememyPrefab),
                static_cast<size_t>((GRID_Y * CELL_SIZE) / SPACESHIP_SIZE)
            ));

    // create spawn profiles
    auto randomSpawnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        PurpleEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::RandomSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::DirectionInitializer>(
        helios::engine::runtime::spawn::behavior::initializers::Direction::Random
        )
    );
    
    auto blueSpawnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        BlueEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::RandomSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::DirectionInitializer>(
        helios::engine::runtime::spawn::behavior::initializers::Direction::Random
        )
    );
    
    auto columnSpawnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        OrangeEnemyPoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::ColumnSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::DirectionInitializer>(
            helios::math::X_AXISf
        )
    );

    auto projectileSpawnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        ProjectilePoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::EmitterSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::EmitterInitializer>()
    );

    auto purpleEnemySpawnScheduler = std::make_unique<helios::engine::runtime::spawn::scheduling::SpawnScheduler>();
    auto purpleEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(5.0f),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(1),
        PurpleEnemySpawnConditionId
    );

    auto orangeEnemySpawnScheduler = std::make_unique<helios::engine::runtime::spawn::scheduling::SpawnScheduler>();
    auto orangeEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(2.0f),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::SpawnAll>(),
        OrangeEnemySpawnConditionId
    );
    
    auto blueEnemySpawnScheduler = std::make_unique<helios::engine::runtime::spawn::scheduling::SpawnScheduler>();
    auto blueEnemySpawnRule = std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
        std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(5.0f),
        std::make_unique<helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(1),
        BlueEnemySpawnConditionId
    );

    // let the SpawnManager know which SpawnProfiles to manage
    spawnManager.addSpawnProfile(RandomSpawnSpawnProfileId, std::move(randomSpawnSpawnProfile));
    spawnManager.addSpawnProfile(VerticalSpawnSpawnProfileId, std::move(columnSpawnSpawnProfile));
    spawnManager.addSpawnProfile(BlueSpawnSpawnProfileId, std::move(blueSpawnSpawnProfile));
    spawnManager.addSpawnProfile(ProjectileSpawnSpawnProfileId, std::move(projectileSpawnSpawnProfile));
    
    // bind the spawn rules to the SpawnProfiles
    purpleEnemySpawnScheduler->addRule(RandomSpawnSpawnProfileId, std::move(purpleEnemySpawnRule));
    orangeEnemySpawnScheduler->addRule(VerticalSpawnSpawnProfileId, std::move(orangeEnemySpawnRule));
    orangeEnemySpawnScheduler->addRule(BlueSpawnSpawnProfileId, std::move(blueEnemySpawnRule));



    // --------------------------
    // --------------------------


    // register gameplay specific dispatchers that delegate commands to Managers
    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::ScheduledSpawnPlanCommandDispatcher>()
    );
    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::SpawnCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::SpawnCommandDispatcher>()
    );
    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::DespawnCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::DespawnCommandDispatcher>()
    );

    // ... systems.

    //                   +----------+
    //                   |  FRAME N |
    //                   +----------+
    //
    //         UPDATE   -   RESOLVE  -   CLEANUP
    //
    // +----------------------------------------------
    // | 1. PHASE ONE: Simulation and Intent Updates
    // +----------------------------------------------

    auto spawnSchedulers = std::vector<std::unique_ptr<helios::engine::runtime::spawn::scheduling::SpawnScheduler>>();
    spawnSchedulers.push_back(std::move(purpleEnemySpawnScheduler));
    spawnSchedulers.push_back(std::move(orangeEnemySpawnScheduler));

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Pre)
            .addPass()
            .addSystem<helios::engine::mechanics::input::systems::TwinStickInputSystem>(*shipGameObject)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass()
            .addSystem<helios::engine::mechanics::spawn::systems::GameObjectSpawnSystem>(spawnSchedulers)
            .addSystem<helios::engine::mechanics::combat::systems::ProjectileSpawnSystem>(ProjectileSpawnSpawnProfileId)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass()
            .addSystem<helios::engine::modules::ai::systems::ChaseSystem>()
            .addSystem<helios::engine::modules::spatial::transform::systems::ScaleSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SteeringSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SpinSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::Move2DSystem>();

    // +---------------------------------------------------------------
    // | 2. PHASE TWO: Constraint Checks and event/command derivation
    // +--------------------------------------------------------------
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Main)
            .addPass()
            .addSystem<helios::engine::modules::physics::collision::systems::BoundsUpdateSystem>()
            .addSystem<helios::engine::mechanics::bounds::systems::LevelBoundsBehaviorSystem>()
            .addSystem<helios::engine::modules::physics::collision::systems::GridCollisionDetectionSystem>(
                levelPtr->bounds(), CELL_SIZE/2.0f
             )
            .addCommitPoint()
            .addPass()
            .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateResponseSystem>();

    // +----------------------------------------------------------------
    // | 3. PHASE THREE: Postprocessing and constraint integration
    // +----------------------------------------------------------------
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Post)
             .addPass()
             .addSystem<helios::engine::modules::spatial::transform::systems::ComposeTransformSystem>()
             .addSystem<helios::engine::modules::systems::scene::SceneSyncSystem>(scene.get())
             .addSystem<helios::engine::modules::spatial::transform::systems::TransformClearSystem>()
             .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateClearSystem>();


    float DELTA_TIME = 0.0f;


    // ========================================
    // 10. Main Game Loop
    // ========================================

    // embedd the immediate game objects
    leftStickGizmo->setActive(true);
    rightStickGizmo->setActive(true);
    shipDirectionGizmo->setActive(true);

    theGrid->setActive(true);
    std::ignore = gameWorld.addGameObject(std::move(theGrid));
    std::ignore = shipGameObject->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode()->addNode(std::move(cameraSceneNode));

    shipGameObject->setActive(true);
    auto* theShipPtr = gameWorld.addGameObject(std::move(shipGameObject));
    auto* leftStickGizmoNode = leftStickGizmo->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* rightStickGizmoNode = rightStickGizmo->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* shipDirectionGizmoNode = shipDirectionGizmo->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();

    // warm up world!
    // ENGINE INIT
    gameWorld.init();
    gameLoop.init(gameWorld);

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

        // ----------------------------------------
        // 10.2 Game Logic Update
        // ----------------------------------------
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = helios::input::InputSnapshot(gamepadState);

        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot);


        // ----------------------------------------
        // 10.3 Gizmo / Debug Visualization Update
        // ----------------------------------------
        const auto* mcLft = theShipPtr->get<helios::engine::modules::physics::motion::components::Move2DComponent>();
        if (mcLft) {
            leftStickGizmoNode->setScale((mcLft->direction() * mcLft->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mcLft->velocity().normalize() * mcLft->speedRatio() * 4.0f);
        }
        const auto* mcRgt = theShipPtr->get<helios::engine::mechanics::combat::components::Aim2DComponent>();
        if (mcRgt) {
            rightStickGizmoNode->setScale((mcRgt->direction() * mcRgt->frequency()  * 4.0f).toVec3());
        }

        // ----------------------------------------
        // 10.4 Rendering
        // ----------------------------------------
        const auto& snapshot = scene->createSnapshot(mainViewport);
        if (snapshot.has_value()) {
            auto renderPass = RenderPassFactory::getInstance().buildRenderPass(*snapshot);
            app->renderingDevice().render(renderPass);
        }

        // ========================================
        // ImGui Rendering
        // ========================================
        imguiOverlay.render();

        // ----------------------------------------
        // 10.5 Frame Synchronization
        // ----------------------------------------
        // swap time / idle time should be read out here
        win->swapBuffers();

        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        DELTA_TIME = frameStats.totalFrameTime;
    }

    return EXIT_SUCCESS;
}
