/**
 * @file registry.ixx
 * @brief Component registration for bounds module.
 */
module;

export module helios.engine.mechanics.bounds.registry;

import helios.ecs.ComponentReflector;
import helios.engine.mechanics.bounds.components;

export namespace helios::engine::mechanics::bounds {

    /**
     * @brief Registers all bounds components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::LevelBoundsBehaviorComponent<typename TEntityManager::Handle_type>>();
    }

}

