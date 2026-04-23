/**
 * @file registry.ixx
 * @brief Component registration for spatial transform module.
 */
module;

export module helios.spatial.registry;

import helios.ecs.ComponentReflector;
import helios.spatial.components;

export namespace helios::spatial {

    /**
     * @brief Registers all spatial transform components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::ComposeTransformComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::WorldTransformStateComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::LocalTransformComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::TranslationStateComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::ScaleStateComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::DimensionComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::RotationStateComponent<typename TEntityManager::Handle_type>>();
    }

}

