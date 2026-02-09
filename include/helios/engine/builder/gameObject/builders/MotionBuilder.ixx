/**
 * @file MotionBuilder.ixx
 * @brief Builder for motion-related component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.MotionBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.Move2DConfig;
import helios.engine.builder.gameObject.builders.configs.SteeringConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring movement and steering components.
     *
     * Provides factory methods for obtaining config objects that
     * set up 2D movement and rotation behavior on a GameObject.
     */
    class MotionBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a MotionBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit MotionBuilder(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for 2D movement setup.
         *
         * @return Move2DConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::Move2DConfig move2D() {
            return helios::engine::builder::gameObject::builders::configs::Move2DConfig{gameObject_};
        }

        /**
         * @brief Returns a config for steering/rotation setup.
         *
         * @return SteeringConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::SteeringConfig steering() const {
            return helios::engine::builder::gameObject::builders::configs::SteeringConfig{gameObject_};
        }

    };

}

