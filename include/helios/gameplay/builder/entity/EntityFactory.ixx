/**
 * @file EntityFactory.ixx
 * @brief Factory for creating and cloning Entities with fluent configuration.
 */
module;

#include <memory>
#include <functional>

export module helios.gameplay.builder.entity.EntityFactory;


import helios.gameplay.builder.entity.builders;

import helios.runtime.world.GameWorld;

import helios.runtime.pooling.components.PrefabIdComponent;

import helios.gameplay.ai;
import helios.gameplay.effects;
import helios.gameplay.bounds;
import helios.gameplay.combat;
import helios.gameplay.spawn;
import helios.gameplay.input;
import helios.gameplay.lifecycle;
import helios.gameplay.damage;
import helios.gameplay.health;
import helios.gameplay.scoring;
import helios.gameplay.gamestate;
import helios.gameplay.matchstate;
import helios.runtime.timing;

import helios.util.Guid;
import helios.core.units;
import helios.gameplay.common.types.PrefabId;
import helios.math;

export namespace helios::gameplay::builder::entity {

    /**
     * @brief Factory class for creating and cloning Entities.
     *
     * @details Provides static methods for constructing new Entities or creating
     * copies from existing templates. Uses the builder pattern internally
     * via `EntityPrototype` for fluent component configuration.
     *
     * The factory supports domain-specific builders for different component categories:
     * motion, rendering, collision, transform, effects, spawn, AI, combat, lifecycle,
     * health, scoring, observer, and menu.
     *
     * ## Usage
     *
     * ```cpp
     * auto player = EntityFactory::entity(gameWorld)
     *     .withMotion([](auto& m) { m.move2D().speed(5.0f); })
     *     .withRendering([&](auto& r) { r.renderable().shape(shape).shader(shader).build(); })
     *     .withCollision([](auto& c) { c.aabb().layer(CollisionLayer::Player); })
     *     .make(true);
     * ```
     *
     * @see EntityPrototype
     * @see MotionBuilder
     * @see RenderingBuilder
     * @see CollisionBuilder
     */
    template<typename Entity, typename World>
    class EntityFactory {

        /**
         * @brief Internal builder class for fluent Entity configuration.
         *
         * Holds domain-specific builders and provides callback-based methods
         * for configuring different aspects of a Entity.
         */
        class EntityPrototype {

            using Handle_type = typename Entity::Handle_type;

            /**
             * @brief The Entity being configured.
             */
            Entity entity_;

            World& gameWorld_;

            /**
             * @brief Builder for motion-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::MotionBuilder<Entity>> motionBuilder_;

            /**
             * @brief Builder for rendering-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::RenderingBuilder<Entity>> renderingBuilder_;

            /**
             * @brief Builder for scene-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::SceneBuilder<Entity>> sceneBuilder_;

            /**
             * @brief Builder for collision-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::CollisionBuilder<Entity>> collisionBuilder_;

            /**
             * @brief Builder for spatial transform components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::TransformBuilder<Entity>> transformBuilder_;

            /**
             * @brief Builder for UI transform components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::UiTransformBuilder<Entity>> uiTransformBuilder_;

            /**
             * @brief Builder for visual effects components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::EffectsBuilder<Entity>> effectsBuilder_;

            /**
             * @brief Builder for spawn-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::SpawnBuilder<Entity>> spawnBuilder_;

            /**
             * @brief Builder for AI behavior components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::AiBuilder<Entity>> aiBuilder_;

            /**
             * @brief Builder for combat-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::CombatBuilder<Entity>> combatBuilder_;

            /**
             * @brief Builder for lifecycle components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::LifecycleBuilder<Entity>> lifecycleBuilder_;

            /**
             * @brief Builder for health-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::HealthBuilder<Entity>> healthBuilder_;

            /**
             * @brief Builder for scoring-related components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::ScoringBuilder<Entity>> scoringBuilder_;

            /**
             * @brief Builder for observer components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::ObserverBuilder<Entity>> observerBuilder_;

            /**
             * @brief Builder for menu components.
             */
            std::unique_ptr<helios::gameplay::builder::entity::builders::MenuBuilder<Entity>> menuBuilder_;
        public:

            /** @brief Callback type for motion configuration. */
            using MotionBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::MotionBuilder<Entity>&)>;

            /** @brief Callback type for rendering configuration. */
            using RenderingBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::RenderingBuilder<Entity>&)>;

            /** @brief Callback type for scene configuration. */
            using SceneBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::SceneBuilder<Entity>&)>;

            /** @brief Callback type for transform configuration. */
            using TransformBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::TransformBuilder<Entity>&)>;

            /** @brief Callback type for UI transform configuration. */
            using UiTransformBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::UiTransformBuilder<Entity>&)>;

            /** @brief Callback type for collision configuration. */
            using CollisionBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::CollisionBuilder<Entity>&)>;

            /** @brief Callback type for effects configuration. */
            using EffectsBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::EffectsBuilder<Entity>&)>;

            /** @brief Callback type for spawn configuration. */
            using SpawnBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::SpawnBuilder<Entity>&)>;

            /** @brief Callback type for AI configuration. */
            using AiBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::AiBuilder<Entity>&)>;

            /** @brief Callback type for combat configuration. */
            using CombatBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::CombatBuilder<Entity>&)>;

            /** @brief Callback type for lifecycle configuration. */
            using LifecycleBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::LifecycleBuilder<Entity>&)>;

            /** @brief Callback type for health configuration. */
            using HealthBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::HealthBuilder<Entity>&)>;

            /** @brief Callback type for scoring configuration. */
            using ScoringBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::ScoringBuilder<Entity>&)>;

            /** @brief Callback type for observer configuration. */
            using ObserverBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::ObserverBuilder<Entity>&)>;

            /** @brief Callback type for menu configuration. */
            using MenuBuilderCallback = std::function<void(helios::gameplay::builder::entity::builders::MenuBuilder<Entity>&)>;

            /**
             * @brief Constructs a prototype with a new empty Entity.
             */
            EntityPrototype(World& gameWorld) :
                entity_(gameWorld.addEntity()),
                gameWorld_(gameWorld),
                motionBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::MotionBuilder<Entity>>(entity_)),
                renderingBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::RenderingBuilder<Entity>>(entity_)),
                sceneBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::SceneBuilder<Entity>>(entity_)),
                collisionBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::CollisionBuilder<Entity>>(entity_)),
                transformBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::TransformBuilder<Entity>>(entity_)),
                uiTransformBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::UiTransformBuilder<Entity>>(entity_)),
                effectsBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::EffectsBuilder<Entity>>(entity_)),
                spawnBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::SpawnBuilder<Entity>>(entity_)),
                aiBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::AiBuilder<Entity>>(entity_)),
                combatBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::CombatBuilder<Entity>>(entity_)),
                lifecycleBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::LifecycleBuilder<Entity>>(entity_)),
                healthBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::HealthBuilder<Entity>>(entity_)),
                scoringBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::ScoringBuilder<Entity>>(entity_)),
                observerBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::ObserverBuilder<Entity>>(entity_)),
                menuBuilder_(std::make_unique<helios::gameplay::builder::entity::builders::MenuBuilder<Entity>>(entity_))

            {}

            /**
             * @brief Returns a reference to this prototype.
             *
             * @return Reference to this prototype.
             */
            EntityPrototype& prototype() {
                return *this;
            }

            /**
             * @brief Finalizes the prototype and returns the configured Entity.
             *
             * @param active Whether the Entity should be active upon creation.
             *
             * @return Ownership of the configured Entity.
             */
            Entity make(const bool active = false) {
                entity_.setActive(active);
                return entity_;
            }



            /**
             * @brief Configures motion-related components.
             *
             * @param mcb Callback receiving a MotionBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withMotion(const MotionBuilderCallback& mcb) {
                mcb(*motionBuilder_);
                return *this;
            }

            /**
             * @brief Configures rendering-related components.
             *
             * @param rbc Callback receiving a RenderingBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withRendering(const RenderingBuilderCallback& rbc) {
                rbc(*renderingBuilder_);
                return *this;
            }

            /**
             * @brief Configures scene-related components.
             *
             * @param sbc Callback receiving a SceneBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withScene(const SceneBuilderCallback& sbc) {
                sbc(*sceneBuilder_);
                return *this;
            }

            /**
             * @brief Configures collision-related components.
             *
             * @param cbc Callback receiving a CollisionBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withCollision(const CollisionBuilderCallback& cbc) {
                cbc(*collisionBuilder_);
                return *this;
            }

            /**
             * @brief Configures spatial transform components.
             *
             * @param tbc Callback receiving a TransformBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withTransform(const TransformBuilderCallback& tbc) {
                tbc(*transformBuilder_);
                return *this;
            }

            /**
             * @brief Configures UI transform components for viewport-relative positioning.
             *
             * @param utbc Callback receiving a UiTransformBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withUiTransform(const UiTransformBuilderCallback& utbc) {
                utbc(*uiTransformBuilder_);
                return *this;
            }

            /**
             * @brief Configures visual effects components.
             *
             * @param ebc Callback receiving an EffectsBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withEffects(const EffectsBuilderCallback& ebc) {
                ebc(*effectsBuilder_);
                return *this;
            }

            /**
             * @brief Configures spawn-related components.
             *
             * @param sbc Callback receiving a SpawnBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withSpawn(const SpawnBuilderCallback& sbc) {
                sbc(*spawnBuilder_);
                return *this;
            }

            /**
             * @brief Configures AI behavior components.
             *
             * @param aibc Callback receiving an AiBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withAi(const AiBuilderCallback& aibc) {
                aibc(*aiBuilder_);
                return *this;
            }

            /**
             * @brief Configures weapon and shooting components.
             *
             * @param sbc Callback receiving a CombatBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withCombat(const CombatBuilderCallback& sbc) {
                sbc(*combatBuilder_);
                return *this;
            }

            /**
             * @brief Configures lifecycle components.
             *
             * @param sbc Callback receiving a LifecycleBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withLifecycle(const LifecycleBuilderCallback& sbc) {
                sbc(*lifecycleBuilder_);
                return *this;
            }

            /**
             * @brief Configures health-related components.
             *
             * @param hbc Callback receiving a HealthBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withHealth(const HealthBuilderCallback& hbc) {
                hbc(*healthBuilder_);
                return *this;
            }

            /**
             * @brief Configures scoring-related components.
             *
             * @param hbc Callback receiving a ScoringBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withScoring(const ScoringBuilderCallback& hbc) {
                hbc(*scoringBuilder_);
                return *this;
            }

            /**
             * @brief Configures observer components for data binding.
             *
             * @param obc Callback receiving an ObserverBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withObserver(const ObserverBuilderCallback& obc) {
                obc(*observerBuilder_);
                return *this;
            }

            /**
             * @brief Configures menu components.
             *
             * @param obc Callback receiving a MenuBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& withMenu(const MenuBuilderCallback& obc) {
                obc(*menuBuilder_);
                return *this;
            }

            /**
             * @brief Marks this Entity as the player entity in the session.
             *
             * @details Registers the Entity with the session so it can be
             * retrieved via `Session::playerEntity()`. Only one entity should
             * be marked as player per session.
             *
             * @return Reference to this prototype for chaining.
             */
            EntityPrototype& asPlayerEntity() noexcept {
                entity_.template getOrAdd<helios::gameplay::matchstate::components::PlayerComponent<Handle_type>>();
                gameWorld_.session().setPlayerEntityHandle(entity_.handle());
                return *this;
            }

            EntityPrototype& withPrefabId(const helios::gameplay::common::types::PrefabId prefabId) {
                entity_.template add<helios::runtime::pooling::components::PrefabIdComponent<Handle_type>>(prefabId);
                return *this;
            }
        };

    public:

        /**
         * @brief Returns the singleton instance of the factory.
         *
         * @return Reference to the singleton factory.
         */
        static EntityFactory& instance() {
            static EntityFactory inst{};
            return inst;
        }

        /**
         * @brief Creates a new empty Entity prototype.
         *
         * @param gameWorld The world where the Entity lives.
         *
         * @return A prototype for fluent configuration.
         */
        static EntityPrototype entity(World& gameWorld) {
            return EntityPrototype(gameWorld);
        }

    };

}
