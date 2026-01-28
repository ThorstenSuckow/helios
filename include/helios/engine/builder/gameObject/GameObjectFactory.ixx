/**
 * @file GameObjectFactory.ixx
 * @brief Factory for creating and cloning GameObjects with fluent configuration.
 */
module;

#include <memory>
#include <functional>

export module helios.engine.builder.gameObject.GameObjectFactory;

import helios.engine.ecs.GameObject;
import helios.engine.ecs.CloneableComponent;
import helios.engine.builder.gameObject.builders;


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
            std::unique_ptr<helios::engine::ecs::GameObject> gameObject_{};

            std::unique_ptr<helios::engine::builder::gameObject::builders::MotionBuilder> motionBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::RenderingBuilder> renderingBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::SceneBuilder> sceneBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::CollisionBuilder> collisionBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::TransformBuilder> transformBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::EffectsBuilder> effectsBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::SpawnBuilder> spawnBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::AiBuilder> aiBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::ShootingBuilder> shootingBuilder_;

            std::unique_ptr<helios::engine::builder::gameObject::builders::LifecycleBuilder> lifecycleBuilder_;

        public:

            using MotionBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::MotionBuilder&)>;

            using RenderingBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::RenderingBuilder&)>;

            using SceneBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::SceneBuilder&)>;

            using TransformBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::TransformBuilder&)>;

            using CollisionBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::CollisionBuilder&)>;

            using EffectsBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::EffectsBuilder&)>;

            using SpawnBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::SpawnBuilder&)>;

            using AiBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::AiBuilder&)>;

            using ShootingBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::ShootingBuilder&)>;

            using LifecycleBuilderCallback = std::function<void(helios::engine::builder::gameObject::builders::LifecycleBuilder&)>;

            explicit GameObjectPrototype(std::unique_ptr<helios::engine::ecs::GameObject> gameObject) :
            gameObject_(std::move(gameObject)),
            motionBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::MotionBuilder>(gameObject_.get())),
            renderingBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::RenderingBuilder>(gameObject_.get())),
            sceneBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::SceneBuilder>(gameObject_.get())),
            collisionBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::CollisionBuilder>(gameObject_.get())),
            transformBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::TransformBuilder>(gameObject_.get())),
            effectsBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::EffectsBuilder>(gameObject_.get())),
            spawnBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::SpawnBuilder>(gameObject_.get())),
            aiBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::AiBuilder>(gameObject_.get())),
            shootingBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::ShootingBuilder>(gameObject_.get())),
            lifecycleBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::LifecycleBuilder>(gameObject_.get()))
            {}

            GameObjectPrototype() :
                gameObject_(std::make_unique<helios::engine::ecs::GameObject>(false)),
                motionBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::MotionBuilder>(gameObject_.get())),
                renderingBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::RenderingBuilder>(gameObject_.get())),
                sceneBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::SceneBuilder>(gameObject_.get())),
                collisionBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::CollisionBuilder>(gameObject_.get())),
                transformBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::TransformBuilder>(gameObject_.get())),
                effectsBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::EffectsBuilder>(gameObject_.get())),
                spawnBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::SpawnBuilder>(gameObject_.get())),
                aiBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::AiBuilder>(gameObject_.get())),
                shootingBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::ShootingBuilder>(gameObject_.get())),
                lifecycleBuilder_(std::make_unique<helios::engine::builder::gameObject::builders::LifecycleBuilder>(gameObject_.get()))
            {}

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
            std::unique_ptr<helios::engine::ecs::GameObject> make(const bool active = false) {
                gameObject_->setActive(active);
                return std::move(gameObject_);
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
             * @param sbc Callback receiving a ShootingBuilder reference.
             *
             * @return Reference to this prototype for chaining.
             */
            GameObjectPrototype& withShooting(const ShootingBuilderCallback& sbc) {
                sbc(*shootingBuilder_);
                return *this;
            }

            /*
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
         * @brief Creates a prototype by cloning an existing GameObject.
         *
         * Copies all components that implement the `Cloneable` interface.
         * The new GameObject starts inactive.
         *
         * @param gameObject Source GameObject to clone from.
         *
         * @return A prototype for further configuration.
         */
        static GameObjectPrototype from(helios::engine::ecs::GameObject* gameObject) {

            auto newGo = std::make_unique<helios::engine::ecs::GameObject>();
            /**
             * @todo Optional ClonePolicy where rules are specified?
             */
            newGo->setActive(false);

            for (const auto* component : gameObject->components()) {
                if (const auto* cc = dynamic_cast<const helios::engine::ecs::Cloneable*>(component)) {
                    auto cComponent = cc->clone();
                    // use getOrAdd since cloned components may have already added
                    // other components in onAttach(), which we will not defer for now.
                    // we assume that the cloned component that allows for adding
                    // components in onAttach is the source of truth for said components
                    newGo->getOrAdd(std::move(cComponent));
                } else {
                    //logger_.warn("Component not cloneable.");
                }

            }

            return GameObjectPrototype(std::move(newGo));

        }

        /**
         * @brief Creates a new empty GameObject prototype.
         *
         * @return A prototype for fluent configuration.
         */
        static GameObjectPrototype gameObject() {
            return {};
        }

    };

}
