/**
 * @file registry.ixx
 * @brief Component registration for model module.
 */
module;

export module helios.rendering.model.registry;

import helios.ecs.ComponentReflector;
import helios.rendering.model.components;

export namespace helios::rendering::model {

    /**
     * @brief Registers all model components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::ModelAabbComponent<typename TEntityManager::Handle_type>>();
    }

}

