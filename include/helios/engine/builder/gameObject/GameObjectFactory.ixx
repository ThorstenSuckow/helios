/**
 * @file GameObjectFactory.ixx
 * @brief Factory for creating and cloning GameObjects with fluent configuration.
 */
module;

#include <memory>
#include <functional>

export module helios.engine.builder.gameObject.GameObjectFactory;

import helios.engine.ecs.GameObject;

import helios.engine.builder.gameObject.builders;

import helios.engine.runtime.world.GameWorld;

import helios.engine.modules;
import helios.engine.mechanics;
import helios.util.Guid;
import helios.engine.core.units;
import helios.math;

export namespace helios::engine::builder::gameObject {

    /**
     * @brief Factory class for creating and cloning GameObjects.
     *
     * Provides static methods for constructing new GameObjects or creating
     * copies from existing templates. Uses the builder pattern internally
     * via `GameObjectPrototype` for fluent component configuration.
     *
     * Example usage:
     * ```cpp
     * auto player = GameObjectFactory::gameObject()
     *     .withMotion([](auto& m) { m.move2D().speed(5.0f); })
     *     .withRendering([&](auto& r) { r.renderable().shape(shape).shader(shader).build(); })
     *     .make(true);
     * ```
     */
    class GameObjectFactory {

        /**
         * @brief Internal builder class for fluent GameObject configuration.
         *
         * Holds domain-specific builders and provides callback-based methods
         * for configuring different aspects of a GameObject.
         */
        class GameObjectPrototype {

            /**
             * @brief The GameObject being configured.
             */
            helios::engine::ecs::GameObject gameObject_;

            /**
             * @brief Builder for motion-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::MotionBuilder> motionBuilder_;

            /**
             * @brief Builder for rendering-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::RenderingBuilder> renderingBuilder_;

            /**
             * @brief Builder for scene-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::SceneBuilder> sceneBuilder_;

            /**
             * @brief Builder for collision-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::CollisionBuilder> collisionBuilder_;

            /**
             * @brief Builder for spatial transform components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::TransformBuilder> transformBuilder_;

            /**
             * @brief Builder for UI transform components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::UiTransformBuilder> uiTransformBuilder_;

            /**
             * @brief Builder for visual effects components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::EffectsBuilder> effectsBuilder_;

            /**
             * @brief Builder for spawn-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::SpawnBuilder> spawnBuilder_;

            /**
             * @brief Builder for AI behavior components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::AiBuilder> aiBuilder_;

            /**
             * @brief Builder for combat-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::CombatBuilder> CombatBuilder_;

            /**
             * @brief Builder for lifecycle components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::LifecycleBuilder> lifecycleBuilder_;

            /**
             * @brief Builder for health-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::HealthBuilder> healthBuilder_;

            /**
             * @brief Builder for scoring-related components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::ScoringBuilder> scoringBuilder_;

            /**
             * @brief Builder for observer components.
             */
            std::unique_ptr<helios::engine::builder::gameObject::builders::ObserverBuilder> observerBuilder_;
        public:

            /** @brief Callback type for motion configuration. */
            using MotionBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::MotionBuilder&)>;

            /** @brief Callback type for rendering configuration. */
            using RenderingBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::RenderingBuilder&)>;

            /** @brief Callback type for scene configuration. */
            using SceneBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::SceneBuilder&)>;

            /** @brief Callback type for transform configuration. */
            using TransformBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::TransformBuilder&)>;

            /** @brief Callback type for UI transform configuration. */
            using UiTransformBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::UiTransformBuilder&)>;

            /** @brief Callback type for collision configuration. */
            using CollisionBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::CollisionBuilder&)>;

            /** @brief Callback type for effects configuration. */
            using EffectsBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::EffectsBuilder&)>;

            /** @brief Callback type for spawn configuration. */
            using SpawnBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::SpawnBuilder&)>;

            /** @brief Callback type for AI configuration. */
            using AiBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::AiBuilder&)>;

            /** @brief Callback type for combat configuration. */
            using CombatBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::CombatBuilder&)>;

            /** @brief Callback type for lifecycle configuration. */
            using LifecycleBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::LifecycleBuilder&)>;

            /** @brief Callback type for health configuration. */
            using HealthBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::HealthBuilder&)>;

            /** @brief Callback type for scoring configuration. */
            using ScoringBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::ScoringBuilder&)>;

            /** @brief Callback type for observer configuration. */
            using ObserverBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::ObserverBuilder&)>;


            /**
             * @brief Constructs a prototype with a new empty GameObject.
             */
            GameObjectPrototype(helios::engine::runtime::world::GameWorld& gameWorld) :
                gameObject_(gameWorld.addGameObject()),
                motionBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::MotionBuilder>(gameObject_)),
                renderingBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::RenderingBuilder>(gameObject_)),
                sceneBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::SceneBuilder>(gameObject_)),
                collisionBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::CollisionBuilder>(gameObject_)),
                transformBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::TransformBuilder>(gameObject_)),
                uiTransformBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::UiTransformBuilder>(gameObject_)),
                effectsBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::EffectsBuilder>(gameObject_)),
                spawnBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::SpawnBuilder>(gameObject_)),
                aiBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::AiBuilder>(gameObject_)),
                CombatBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::CombatBuilder>(gameObject_)),
                lifecycleBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::LifecycleBuilder>(gameObject_)),
                healthBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::HealthBuilder>(gameObject_)),
                scoringBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::ScoringBuilder>(gameObject_)),
                observerBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::ObserverBuilder>(gameObject_))

            {}

            /**
             * @brief Returns a reference to this prototype.
             *
             * @return Reference to this prototype.
             */
            GameObjectPrototype& prototype() {
                return *this;
            }

            /**
             * @brief Finalizes the prototype and returns the configured GameObject.
             *
             * @param active Whether the GameObject should be active upon creation.
             *
             * @return Ownership of the configured GameObject.
             */
            helios::engine::ecs::GameObject make(const bool active = false) {
                gameObject_.setActive(active);
                return gameObject_;
            }



            /**
             * @brief Configures motion-related components.
             *
             * @param mcb Callback receiving a MotionBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            GameObjectPrototype& withMotion(const MotionBuilderCallback& mcb) {
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
            GameObjectPrototype& withRendering(const RenderingBuilderCallback& rbc) {
                rbc(*renderingBuilder_);
                return *this;
            }

            /**
             * @brief Configures collision-related components.
             *
             * @param cbc Callback receiving a CollisionBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            GameObjectPrototype& withCollision(const CollisionBuilderCallback& cbc) {
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
            GameObjectPrototype& withTransform(const TransformBuilderCallback& tbc) {
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
            GameObjectPrototype& withUiTransform(const UiTransformBuilderCallback& utbc) {
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
            GameObjectPrototype& withEffects(const EffectsBuilderCallback& ebc) {
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
            GameObjectPrototype& withSpawn(const SpawnBuilderCallback& sbc) {
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
            GameObjectPrototype& withAi(const AiBuilderCallback& aibc) {
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
            GameObjectPrototype& withCombat(const CombatBuilderCallback& sbc) {
                sbc(*CombatBuilder_);
                return *this;
            }

            /**
             * @brief Configures lifecycle components.
             *
             * @param sbc Callback receiving a LifecycleBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            GameObjectPrototype& withLifecycle(const LifecycleBuilderCallback& sbc) {
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
            GameObjectPrototype& withHealth(const HealthBuilderCallback& hbc) {
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
            GameObjectPrototype& withScoring(const ScoringBuilderCallback& hbc) {
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
            GameObjectPrototype& withObserver(const ObserverBuilderCallback& obc) {
                obc(*observerBuilder_);
                return *this;
            }

        };

    public:

        /**
         * @brief Returns the singleton instance of the factory.
         *
         * @return Reference to the singleton factory.
         */
        static GameObjectFactory& instance() {
            static GameObjectFactory inst{};
            return inst;
        }

        /**
         * @brief Creates a new empty GameObject prototype.
         *
         * @param gameWorld The world where the GameObject lives.
         *
         * @return A prototype for fluent configuration.
         */
        static GameObjectPrototype gameObject(helios::engine::runtime::world::GameWorld& gameWorld) {
            return GameObjectPrototype(gameWorld);
        }

    };

}
