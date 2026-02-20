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
import helios.examples.collisionDetection;


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
using namespace helios::engine::state;


// ============================================================================
// Entry Point
// ============================================================================

int main() {

    // ========================================
    // 1. Constants
    // ========================================
    constexpr float CELL_LENGTH          = 5.0f;
    constexpr float SPACESHIP_LENGTH     = 5.0f;
    constexpr float GRID_X             = 29.0f;
    constexpr float GRID_Y             = 19.0f;
    constexpr float FOVY               = helios::math::radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;

    constexpr helios::math::vec3f GRID_SCALE{GRID_X * CELL_LENGTH, GRID_Y * CELL_LENGTH, 0.0f};

    constexpr auto TOP_LEFT_COORDINATES = helios::math::vec3f(GRID_SCALE[0] * -0.5f, GRID_SCALE[1] * 0.5f, 0.0f);
    constexpr auto TOP_RIGHT_COORDINATES = helios::math::vec3f(GRID_SCALE[0] * 0.5f, GRID_SCALE[1] * 0.5f, 0.0f);
    constexpr auto BOTTOM_LEFT_COORDINATES =  helios::math::vec3f(GRID_SCALE[0] * -0.5f, GRID_SCALE[1] * -0.5f, 0.0f);
    constexpr auto BOTTOM_RIGHT_COORDINATES =  helios::math::vec3f(GRID_SCALE[0] * 0.5f, GRID_SCALE[1] * -0.5f, 0.0f);

    constexpr auto BLUE_ENEMY_SCALE = helios::math::vec3f{SPACESHIP_LENGTH, SPACESHIP_LENGTH/2.0f, 0.0f};
    constexpr auto SPACESHIP_SPAWN_POSITION = helios::math::vec3f(0.0f, 0.0f, 0.0f);
    constexpr auto SPACESHIP_SIZE = helios::math::vec3f(SPACESHIP_LENGTH, SPACESHIP_LENGTH, 0.0f);

    constexpr size_t OBJECT_AMOUNT_X = GRID_X * CELL_LENGTH / SPACESHIP_LENGTH;
    constexpr size_t OBJECT_AMOUNT_Y = GRID_Y * CELL_LENGTH / SPACESHIP_LENGTH;

    constexpr helios::engine::core::data::PrefabId ProjectilePrefabId{"projectile"};
    constexpr helios::engine::core::data::PrefabId PurpleEnemyPrefabId{"purple_enemy"};
    constexpr helios::engine::core::data::PrefabId OrangeEnemyPrefabId{"orange_enemy"};
    constexpr helios::engine::core::data::PrefabId BlueEnemyPrefabId{"blue_enemy"};

    constexpr helios::engine::core::data::GameObjectPoolId ProjectilePoolId{"projectile_pool"};
    constexpr helios::engine::core::data::GameObjectPoolId PurpleEnemyPoolId{"purple_pool"};
    constexpr helios::engine::core::data::GameObjectPoolId OrangeEnemyPoolId{"orange_pool"};
    constexpr helios::engine::core::data::GameObjectPoolId BlueEnemyPoolId{"blue_pool"};

    constexpr helios::engine::core::data::SpawnProfileId ProjectileSpawnSpawnProfileId{"projectile_spawn"};
    constexpr helios::engine::core::data::SpawnProfileId RandomSpawnSpawnProfileId{"random_spawn"};
    constexpr helios::engine::core::data::SpawnProfileId BlueMassSpawnProfileId{"blue_spawn_profile1"};
    constexpr helios::engine::core::data::SpawnProfileId LeftColumnSpawnProfileId{"orange_left_spawn1"};
    constexpr helios::engine::core::data::SpawnProfileId RightColumnSpawnProfileId{"orange_right_spawn_1"};
    constexpr helios::engine::core::data::SpawnProfileId TopRowSpawnProfileId{"orange_top_spawn_1"};
    constexpr helios::engine::core::data::SpawnProfileId BottomRowSpawnProfileId{"orange_bottom_spawn_1"};

    constexpr helios::engine::core::data::SpawnRuleId PurpleEnemySpawnConditionId{"purple_spawn_rule"};
    constexpr helios::engine::core::data::SpawnRuleId BlueMassSpawnConditionId{"blue_mass_spawn1"};
    constexpr helios::engine::core::data::SpawnRuleId OrangeSpawnRuleLeftColumn{"orange_spawn_rule_1"};
    constexpr helios::engine::core::data::SpawnRuleId OrangeSpawnRuleTopRow{"orange_spawn_rule_3"};
    constexpr helios::engine::core::data::SpawnRuleId OrangeSpawnRuleRightColumn{"orange_spawn_rule_2"};
    constexpr helios::engine::core::data::SpawnRuleId OrangeSpawnRuleBottomRow{"orange_spawn_rule_4"};

    // ========================================
    // 2. Application and Window Setup
    // ========================================
    helios::engine::bootstrap::registerAllComponents();

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Collision Detection Demo", 1980, 1024, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto sceneToViewportMap = helios::engine::modules::scene::types::SceneToViewportMap();
    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f,
        helios::engine::core::data::ViewportId{"mainViewport"});

    mainViewport->setClearFlags(std::to_underlying(ClearFlags::Color))
                  .setClearColor(vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->addViewport(mainViewport);

    helios::input::InputManager& inputManager = app->inputManager();
    unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);

    const auto basicStringFileReader = BasicStringFileReader();

    // ----------------------------------------
    // 2.1 ImGui and Debug Tooling
    // ----------------------------------------
    auto imguiBackend = helios::ext::imgui::ImGuiGlfwOpenGLBackend(win->nativeHandle());
    auto imguiOverlay = helios::ext::imgui::ImGuiOverlay::forBackend(&imguiBackend);
    auto fpsMetrics = helios::engine::tooling::FpsMetrics();
    auto stopwatch = std::make_unique<helios::util::time::Stopwatch>();
    auto framePacer = helios::engine::tooling::FramePacer(std::move(stopwatch));
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

    // ----------------------------------------
    // 2.2 Logger Configuration
    // ----------------------------------------
    helios::util::log::LogManager::getInstance().enableLogging(true);
    auto imguiLogSink = std::make_shared<helios::ext::imgui::ImGuiLogSink>(logWidget);
    helios::util::log::LogManager::getInstance().enableSink(imguiLogSink);

    // ========================================
    // 3. Shader Creation
    // ========================================
    auto defaultShader = std::make_shared<OpenGLShader>(
        "./resources/cube.vert", "./resources/cube.frag", basicStringFileReader);

    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    bool mapping = uniformLocationMap->set(UniformSemantics::ModelMatrix, 1);
    mapping = uniformLocationMap->set(UniformSemantics::ViewMatrix, 2);
    mapping = uniformLocationMap->set(UniformSemantics::ProjectionMatrix, 3);
    mapping = uniformLocationMap->set(UniformSemantics::MaterialBaseColor, 4);

    defaultShader->setUniformLocationMap(std::move(uniformLocationMap));

    // ========================================
    // 4. Scene Graph and Camera Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<helios::scene::Scene>(
        std::move(frustumCullingStrategy), helios::engine::core::data::SceneId{"mainScene"});
    sceneToViewportMap.add(scene.get(), mainViewport.get());

    auto mainViewportCam = std::make_unique<helios::scene::Camera>();
    auto cameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(mainViewportCam));
    auto cameraSceneNode_ptr = cameraSceneNode.get();

    cameraSceneNode_ptr->setInheritance(helios::math::TransformType::Translation);
    mainViewport->setCameraSceneNode(cameraSceneNode_ptr);
    cameraSceneNode_ptr->setTranslation(helios::math::vec3f(0.0f, 0.0f, -100.0f));
    cameraSceneNode_ptr->camera().setPerspective(
        FOVY, ASPECT_RATIO_NUMER / ASPECT_RATIO_DENOM, 0.1f, 1000.0f
    );
    cameraSceneNode_ptr->lookAtLocal(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));
    cameraWidget->addCameraSceneNode("Main Camera", cameraSceneNode_ptr);

    // ========================================
    // 5. GameWorld and Level Setup
    // ========================================
    helios::engine::runtime::gameloop::GameLoop gameLoop{};
    helios::engine::runtime::world::GameWorld gameWorld{};

    gameWorld.session().trackState<helios::engine::mechanics::gamestate::types::GameState>();
    gameWorld.session().trackState<helios::engine::mechanics::match::types::MatchState>();

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

    // ========================================
    // 6. GameObjects
    // ========================================

    // Projectile prefab
    auto projectilePrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withPrefabId(ProjectilePrefabId)
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
              .layerId(helios::examples::collisionDetection::CollisionId::Projectile)
              .useBoundingBox()
              .hitPolicy(helios::engine::modules::physics::collision::types::HitPolicy::OneHit)
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

    // Grid
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

    // Player ship
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
              .layerId(helios::examples::collisionDetection::CollisionId::Player)
              .useBoundingBox()
              .hitPolicy(helios::engine::modules::physics::collision::types::HitPolicy::OneHit)
              .reportCollisions(true)
              .solidCollisionMask(helios::examples::collisionDetection::CollisionId::Enemy)
              .onSolidCollision(
                  helios::examples::collisionDetection::CollisionId::Enemy,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Bounce);
        })
        .withCombat([](auto& ccb) {
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

    // Debug gizmos (input visualization)
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

    // Purple enemy prefab
    auto purpleEnemyPrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withPrefabId(PurpleEnemyPrefabId)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::LimeGreen)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Rectangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_LENGTH/2.0f, SPACESHIP_LENGTH/2.0f, 0.0f));
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

    // Orange enemy prefab
    auto orangeEnemyPrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withPrefabId(OrangeEnemyPrefabId)
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

    // Blue enemy prefab
    auto blueEnemyPrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withPrefabId(BlueEnemyPrefabId)
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
              .speed(shipGameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>()->movementSpeed() * 0.5f)
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

    // ========================================
    // 7. Manager Registration
    // ========================================
    auto& poolManager = gameWorld.addManager<helios::engine::runtime::pooling::GameObjectPoolManager>();
    auto& spawnManager = gameWorld.addManager<helios::engine::runtime::spawn::SpawnManager>();

    // Spawn system
    helios::engine::builder::spawnSystem::SpawnSystemFactory::configure(poolManager, spawnManager)
        // Projectile pool: emitter-relative placement, no scheduling
        .pool(ProjectilePoolId, ProjectilePrefabId, 50)
            .profile(ProjectileSpawnSpawnProfileId)
                .emitterPlacement()
                .done()
            .commit()

        // Purple enemy pool: random placement, scheduled every 5s
        .pool(PurpleEnemyPoolId, PurpleEnemyPrefabId, 50)
            .profile(RandomSpawnSpawnProfileId)
                .randomPlacement()
                .randomDirectionInitializer()
                .scheduledBy(PurpleEnemySpawnConditionId)
                    .timerCondition(5.0f)
                    .fixedAmount(1)
                    .done()
                .done()
            .commit()

        // Blue enemy pool: distributed mass spawn, scheduled every 30s
        .pool(BlueEnemyPoolId, BlueEnemyPrefabId, 100)
            .profile(BlueMassSpawnProfileId)
                .placer(std::make_unique<helios::engine::runtime::spawn::behavior::placements::DistributedSpawnPlacer<4>>(
                    TOP_LEFT_COORDINATES + helios::math::vec3f(CELL_LENGTH, -CELL_LENGTH, 0.0f),
                    TOP_RIGHT_COORDINATES + helios::math::vec3f(-CELL_LENGTH, -CELL_LENGTH, 0.0f),
                    BOTTOM_LEFT_COORDINATES + helios::math::vec3f(CELL_LENGTH, CELL_LENGTH, 0.0f),
                    BOTTOM_RIGHT_COORDINATES + helios::math::vec3f(-CELL_LENGTH, CELL_LENGTH, 0.0f)
                ))
                .initializer(std::make_unique<helios::engine::runtime::spawn::behavior::initializers::InitializerList<2>>(
                    std::make_unique<helios::engine::runtime::spawn::behavior::initializers::DelayedComponentEnablerInitializer<
                        helios::engine::modules::physics::motion::components::Move2DComponent
                    >>(0.5f, 25),
                    std::make_unique<helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
                        helios::math::vec3f{0.0f}, helios::engine::runtime::spawn::behavior::initializers::DirectionType::Point
                    )
                ))
                .scheduledBy(BlueMassSpawnConditionId)
                    .timerWithAvailabilityCondition(30.0f)
                    .fixedAmount(100)
                    .done()
                .done()
            .commit()

        // Orange enemy pool: cyclic edge spawning from four directions
        .pool(OrangeEnemyPoolId, OrangeEnemyPrefabId, OBJECT_AMOUNT_X)
            .profile(LeftColumnSpawnProfileId)
                .axisPlacement(helios::math::vec3f(0.0f, -1.0f, 0.0f).normalize(), TOP_LEFT_COORDINATES)
                .moveInitializer(helios::math::X_AXISf)
                .scheduledBy(OrangeSpawnRuleLeftColumn)
                    .timerWithAvailabilityCondition(15.0f)
                    .fixedAmount(OBJECT_AMOUNT_Y)
                    .done()
                .done()
            .profile(TopRowSpawnProfileId)
                .axisPlacement(helios::math::vec3f(1.0f, 0.0f, 0.0f).normalize(), TOP_LEFT_COORDINATES)
                .moveInitializer(helios::math::Y_AXISf * -1.0f)
                .scheduledBy(OrangeSpawnRuleTopRow)
                    .timerWithAvailabilityCondition(15.0f)
                    .fixedAmount(OBJECT_AMOUNT_X)
                    .done()
                .done()
            .profile(RightColumnSpawnProfileId)
                .axisPlacement(helios::math::vec3f(0.0f, -1.0f, 0.0f).normalize(), TOP_RIGHT_COORDINATES)
                .moveInitializer(helios::math::X_AXISf * -1.0f)
                .scheduledBy(OrangeSpawnRuleRightColumn)
                    .timerWithAvailabilityCondition(15.0f)
                    .fixedAmount(OBJECT_AMOUNT_Y)
                    .done()
                .done()
            .profile(BottomRowSpawnProfileId)
                .axisPlacement(helios::math::vec3f(1.0f, 0.0f, 0.0f).normalize(), BOTTOM_LEFT_COORDINATES)
                .moveInitializer(helios::math::Y_AXISf)
                .scheduledBy(OrangeSpawnRuleBottomRow)
                    .timerWithAvailabilityCondition(15.0f)
                    .fixedAmount(OBJECT_AMOUNT_X)
                    .done()
                .done()
            .commitCyclic<4>();

    // ========================================
    // 8. Command Dispatchers
    // ========================================
    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand>(
                std::make_unique<helios::engine::runtime::spawn::dispatcher::ScheduledSpawnPlanCommandDispatcher>()
            ).addDispatcher<helios::engine::runtime::spawn::commands::SpawnCommand>(
                std::make_unique<helios::engine::runtime::spawn::dispatcher::SpawnCommandDispatcher>()
            ).addDispatcher<helios::engine::runtime::spawn::commands::DespawnCommand>(
                std::make_unique<helios::engine::runtime::spawn::dispatcher::DespawnCommandDispatcher>()
            );

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::core::data;

    // ----------------------------------------
    // 8.1 State-to-Viewport Mapping
    // ----------------------------------------
    auto stateToViewportMap = helios::engine::state::StateToIdMapPair<
        GameState, MatchState, ViewportId
    >();
    stateToViewportMap.add(GameState::Any, ViewportId{"mainViewport"});
    stateToViewportMap.freeze();

    // ========================================
    // 9. GameLoop Phase Configuration
    // ========================================
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Pre)
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::mechanics::input::systems::TwinStickInputSystem>(shipGameObject)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::mechanics::spawn::systems::GameObjectSpawnSystem>(spawnManager)
            .addSystem<helios::engine::mechanics::combat::systems::ProjectileSpawnSystem>(ProjectileSpawnSpawnProfileId)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::modules::ai::systems::ChaseSystem>()
            .addSystem<helios::engine::modules::spatial::transform::systems::ScaleSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SteeringSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SpinSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::Move2DSystem>();

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Main)
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::ecs::systems::HierarchyPropagationSystem>()
            .addSystem<helios::engine::modules::physics::collision::systems::BoundsUpdateSystem>()
            .addSystem<helios::engine::mechanics::bounds::systems::LevelBoundsBehaviorSystem>()
            .addSystem<helios::engine::modules::physics::collision::systems::GridCollisionDetectionSystem>(
                levelPtr->bounds(), CELL_LENGTH/2.0f
             )
            .addCommitPoint()
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateResponseSystem>();

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Post)
             .addPass<GameState>(GameState::Any)
             .addSystem<helios::engine::modules::spatial::transform::systems::ComposeTransformSystem>()
             .addSystem<
                 helios::engine::modules::rendering::viewport::systems::StateToViewportPolicyUpdateSystem
                    <GameState, MatchState>>(stateToViewportMap)
             .addSystem<helios::engine::modules::scene::systems::SceneSyncSystem>(sceneToViewportMap)
             .addSystem<helios::engine::modules::scene::systems::SceneRenderingSystem>(
                 app->renderingDevice(), sceneToViewportMap)
             .addSystem<helios::engine::modules::spatial::transform::systems::TransformClearSystem>()
             .addSystem<helios::engine::mechanics::lifecycle::systems::DelayedComponentEnablerSystem>()
             .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateClearSystem>();

    // ========================================
    // 10. Initialization and Game Loop
    // ========================================
    float DELTA_TIME = 0.0f;

    leftStickGizmo.setActive(true);
    rightStickGizmo.setActive(true);
    shipDirectionGizmo.setActive(true);
    theGrid.setActive(true);

    std::ignore = scene->addNode(std::move(cameraSceneNode));

    shipGameObject.setActive(true);

    auto* leftStickGizmoNode = leftStickGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* rightStickGizmoNode = rightStickGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* shipDirectionGizmoNode = shipDirectionGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();

    gameWorld.init();
    gameLoop.init(gameWorld);

    gameWorld.session().setStateFrom<GameState>(
        StateTransitionContext<GameState>(GameState::Undefined, GameState::Start, GameStateTransitionId::StartRequested)
    );

    bool showImgui = false;
    bool tilde = false;

    while (!win->shouldClose()) {
        framePacer.beginFrame();

        // 10.1 Event and Input Processing
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

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

        // 10.2 Game Logic Update
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = helios::input::InputSnapshot(gamepadState);

        const auto viewportSnapshots = win->viewportSnapshots();
        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot, viewportSnapshots);

        // 10.3 Debug Gizmo Update
        const auto* mcLft = shipGameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
        if (mcLft) {
            leftStickGizmoNode->setScale((mcLft->direction() * mcLft->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mcLft->velocity().normalize() * mcLft->speedRatio() * 4.0f);
        }
        const auto* mcRgt = shipGameObject.get<helios::engine::mechanics::combat::components::Aim2DComponent>();
        if (mcRgt) {
            rightStickGizmoNode->setScale((mcRgt->direction() * mcRgt->frequency()  * 4.0f).toVec3());
        }

        // 10.4 ImGui Overlay
        if (showImgui) {
            imguiOverlay.render();
        }

        // 10.5 Frame Synchronization
        win->swapBuffers();

        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        DELTA_TIME = frameStats.totalFrameTime;
    }

    return EXIT_SUCCESS;
}
