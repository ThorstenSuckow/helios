/**
 * @file MotionBuilder.ixx
 * @brief Builder for motion-related component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.MotionBuilder;

import helios.gameplay.builder.entity.builders.configs.Move2DConfig;
import helios.gameplay.builder.entity.builders.configs.SteeringConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring movement and steering components.
     *
     * Provides factory methods for obtaining config objects that
     * set up 2D movement and rotation behavior on a Entity.
     */
    template<typename Entity>
    class MotionBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a MotionBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit MotionBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for 2D movement setup.
         *
         * @return Move2DConfig for fluent configuration.
         */
        helios::gameplay::builder::entity::builders::configs::Move2DConfig<Entity> move2D() {
            return helios::gameplay::builder::entity::builders::configs::Move2DConfig<Entity>{entity_};
        }

        /**
         * @brief Returns a config for steering/rotation setup.
         *
         * @return SteeringConfig for fluent configuration.
         */
        helios::gameplay::builder::entity::builders::configs::SteeringConfig<Entity> steering() const {
            return helios::gameplay::builder::entity::builders::configs::SteeringConfig<Entity>{entity_};
        }

    };

}
