/**
 * @file registry.ixx
 * @brief Component registration for physics motion module.
 */
module;

export module helios.engine.modules.physics.motion.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.physics.motion.components;

export namespace helios::engine::modules::physics::motion {

    /**
     * @brief Registers all motion components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::DirectionComponent>();
        R::registerType<components::Move2DComponent>();
        R::registerType<components::SteeringComponent>();
    }

}

