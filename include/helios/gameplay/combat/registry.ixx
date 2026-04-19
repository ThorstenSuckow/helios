/**
 * @file registry.ixx
 * @brief Component registration for combat module.
 */
module;

export module helios.gameplay.combat.registry;

import helios.ecs.ComponentReflector;
import helios.gameplay.combat.components;

export namespace helios::gameplay::combat {

    /**
     * @brief Registers all combat components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::ShootComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::Aim2DComponent<typename TEntityManager::Handle_type>>();
    }

}

