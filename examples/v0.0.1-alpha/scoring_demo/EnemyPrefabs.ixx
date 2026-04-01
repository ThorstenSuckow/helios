/**
 * @file EnemyPrefabs.ixx
 * @brief Enemy prefab creation for the scoring demo.
 */
module;

#include <memory>

export module helios.examples.scoring.EnemyPrefabs;

import helios.ext.opengl.rendering.shader.LegacyOpenGLShader;

import helios.examples.scoring.ArenaConfig;
import helios.examples.scoring.IdConfig;
import helios.examples.scoring.CollisionId;

import helios.engine.mechanics;
import helios.engine.modules;

import helios.engine.ecs;

import helios.engine.runtime.world.GameWorld;

import helios.math;

import helios.util.Colors;

import helios.rendering;

import helios.engine.builder.gameObject.GameObjectFactory;

import helios.engine.modules.physics.collision.types;

using namespace helios::engine::modules::physics::collision::types;

using namespace helios::engine::mechanics::health::types;

export namespace helios::examples::scoring {

    /**
     * @brief Creates and registers all enemy prefab GameObjects.
     *
     * @param gameWorld The game world to create prefabs in.
     * @param playerGameObject The player entity (used for AI targeting and speed reference).
     * @param defaultShader The shared mesh shader.
     */
    inline void createEnemyPrefabs(
        helios::engine::runtime::world::GameWorld& gameWorld,
        helios::engine::ecs::GameObject playerGameObject,
        std::shared_ptr<helios::ext::opengl::rendering::shader::LegacyOpenGLShader> defaultShader
    ) {
        using namespace helios::engine::runtime::world;
        using namespace helios::engine::builder::gameObject;
        using namespace helios::engine::ecs;
        using namespace helios::ext::opengl::rendering::shader;

            auto level = gameWorld.level();
             // purple enemy
            auto purpleEnemyPrefab = GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withPrefabId(IdConfig::PurpleEnemyPrefab)
                .withRendering([defaultShader, root = level->rootNode()](auto& rnb) {
                    rnb.meshRenderable()
                       .shader(defaultShader)
                       .color(helios::util::Colors::LimeGreen)
                       .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
                       .shape(std::make_shared<helios::rendering::asset::shape::basic::Rectangle>())
                       .attachTo(root);
                })
                .withHealth([](auto& hsb) {
                    hsb.health()
                       .maxHealth(100.0f)
                       .healthDepletedTriggers(HealthDepletedBehavior::Despawn);
                })
                .withScoring([](builders::ScoringBuilder& sb) {
                    sb.scoreValue()
                      .score<helios::engine::mechanics::scoring::types::KillReward>(50);
                })
                .withCombat([](auto& cb) {
                    cb.combat()
                      .trackLastDamage();
                })
                .withTransform([](auto& tb) {
                    tb.transform()
                      .scale(helios::math::vec3f(ArenaConfig::SPACESHIP_LENGTH/2.0f, ArenaConfig::SPACESHIP_LENGTH/2.0f, 0.0f));
                })
                .withCollision([](auto& cb) {
                    cb.collision()
                      .layerId(CollisionId::Enemy)
                      .useBoundingBox()
                      .hitPolicy(HitPolicy::OneHit)
                      .reportCollisions(true)
                      .dealDamage(100.0f, CollisionId::Player)
                      .solidCollisionMask(CollisionId::Player | CollisionId::Projectile)
                      .onSolidCollision(CollisionId::Player, CollisionBehavior::PassEvent);

                    cb.levelBoundsCollision()
                      .onCollision(CollisionBehavior::Reflect);
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
            auto orangeEnememyPrefab = GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withPrefabId(IdConfig::OrangeEnemyPrefab)
                .withRendering([&defaultShader, root = level->rootNode()](auto& rnb) {
                    rnb.meshRenderable()
                       .shader(defaultShader)
                       .color(helios::util::Colors::Orange)
                       .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
                       .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
                       .attachTo(root);
                })
                .withTransform([](auto& tb) {
                    tb.transform()
                      .scale(helios::math::vec3f(ArenaConfig::SPACESHIP_LENGTH/2.0f, ArenaConfig::SPACESHIP_LENGTH/2.0f, 0.0f));
                })
                .withHealth([](auto& hsb) {
                    hsb.health()
                       .maxHealth(100.0f)
                       .healthDepletedTriggers(HealthDepletedBehavior::Despawn);
                })
                .withScoring([](builders::ScoringBuilder& sb) {
                    sb.scoreValue()
                      .score<helios::engine::mechanics::scoring::types::KillReward>(100);
                })
                .withCombat([](auto& cb) {
                    cb.combat()
                      .trackLastDamage();
                })
                .withCollision([](auto& cb) {
                    cb.collision()
                      .layerId(CollisionId::Enemy)
                      .useBoundingBox()
                      .hitPolicy(HitPolicy::OneHit)
                      .reportCollisions(true)
                      .dealDamage(100.0f, CollisionId::Player)
                      .solidCollisionMask(CollisionId::Player | CollisionId::Projectile)
                      .onSolidCollision(CollisionId::Player, CollisionBehavior::PassEvent);

                    cb.levelBoundsCollision()
                      .restitution(0.0001f)
                      .onCollision(CollisionBehavior::Bounce)
                      .respondWith(CollisionResponse::AlignHeadingToDirection);
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
            auto blueEnemyPrefab = GameObjectFactory::instance()
                .gameObject(gameWorld)
                .withPrefabId(IdConfig::BlueEnemyPrefab)
                .withRendering([&defaultShader, root = level->rootNode()](auto& rnb) {
                    rnb.meshRenderable()
                       .shader(defaultShader)
                       .color(helios::util::Colors::LightBlue)
                       .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
                       .shape(std::make_shared<helios::rendering::asset::shape::basic::Rhombus>())
                       .attachTo(root);
                })
                .withTransform([scale = ArenaConfig::BLUE_ENEMY_SCALE](auto& tb) {
                    tb.transform()
                      .scale(scale);
                })
                .withHealth([](auto& hsb) {
                    hsb.health()
                       .maxHealth(100.0f)
                       .healthDepletedTriggers(HealthDepletedBehavior::Despawn);
                })
                .withScoring([](builders::ScoringBuilder& sb) {
                    sb.scoreValue()
                      .score<helios::engine::mechanics::scoring::types::KillReward>(150);
                })
                .withCombat([](auto& cb) {
                    cb.combat()
                      .trackLastDamage();
                })
                .withCollision([](auto& cb) {
                    cb.collision()
                      .layerId(CollisionId::Enemy)
                      .useBoundingBox()
                      .hitPolicy(HitPolicy::OneHit)
                      .reportCollisions(true)
                      .dealDamage(100.0f, CollisionId::Player)
                      .solidCollisionMask(CollisionId::Player | CollisionId::Projectile)
                      .onSolidCollision(CollisionId::Player, CollisionBehavior::PassEvent);

                    cb.levelBoundsCollision()
                      .onCollision(CollisionBehavior::Reflect)
                      .respondWith(CollisionResponse::AlignHeadingToDirection);
                })
                .withMotion([&](auto& mcb) {
                   mcb.move2D()
                      .speed( playerGameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>()->movementSpeed()  * 0.5f)
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
                       .target(playerGameObject.entityHandle())
                       .seekInterval(0.05f);
                })
                .make();

    }

}