/**
 * @file registry.ixx
 * @brief Component registration for bounds module.
 */
module;

export module helios.gameplay.bounds.registry;

import helios.ecs.ComponentReflector;
import helios.gameplay.bounds.components;

export namespace helios::gameplay::bounds {

    /**
     * @brief Registers all bounds components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::LevelBoundsBehaviorComponent<typename TEntityManager::Handle_type>>();
    }

}

