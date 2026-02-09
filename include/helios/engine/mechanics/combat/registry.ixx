/**
 * @file registry.ixx
 * @brief Component registration for combat module.
 */
module;

export module helios.engine.mechanics.combat.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.mechanics.combat.components;

export namespace helios::engine::mechanics::combat {

    /**
     * @brief Registers all combat components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::ShootComponent>();
        R::registerType<components::LastAttackerComponent>();
        R::registerType<components::Aim2DComponent>();
    }

}

