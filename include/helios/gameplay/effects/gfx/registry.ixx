/**
 * @file registry.ixx
 * @brief Component registration for effects GFX module.
 */
module;

export module helios.gameplay.effects.gfx.registry;

import helios.ecs.ComponentReflector;
import helios.gameplay.effects.gfx.components;

export namespace helios::gameplay::effects::gfx {

    /**
     * @brief Registers all effects GFX components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::SpinComponent<typename TEntityManager::Handle_type>>();
    }

}

