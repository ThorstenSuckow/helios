/**
 * @file registry.ixx
 * @brief Component registration for entire rendering module.
 */
module;

export module helios.rendering.registry;

import helios.ecs.ComponentReflector;


import helios.rendering.model.registry;

import helios.rendering.common.components;

export namespace helios::rendering {

    /**
     * @brief Registers all rendering components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::rendering::model::registerComponents<TEntityManager>();

        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<common::components::RenderableComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<common::components::RenderPrototypeComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<common::components::MaterialOverrideComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<common::components::ClearComponent<typename TEntityManager::Handle_type>>();
    }

}

