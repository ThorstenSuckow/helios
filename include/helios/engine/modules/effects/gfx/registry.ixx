/**
 * @file registry.ixx
 * @brief Component registration for effects GFX module.
 */
module;

export module helios.engine.modules.effects.gfx.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.effects.gfx.components;

export namespace helios::engine::modules::effects::gfx {

    /**
     * @brief Registers all effects GFX components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::SpinComponent>();
    }

}

