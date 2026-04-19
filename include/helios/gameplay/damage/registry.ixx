/**
 * @file registry.ixx
 * @brief Component registration for damage module.
 */
module;

export module helios.gameplay.damage.registry;

import helios.ecs.ComponentReflector;
import helios.gameplay.damage.components;

export namespace helios::gameplay::damage {

    /**
     * @brief Registers all damage components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::DamageDealerComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::LastDamageComponent<typename TEntityManager::Handle_type>>();
    }

}

