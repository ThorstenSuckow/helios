/**
 * @file registry.ixx
 * @brief Component registration for scene module.
 */
module;

export module helios.scene.registry;

import helios.ecs.ComponentReflector;
import helios.scene.components;

export namespace helios::scene {

    /**
     * @brief Registers all scene components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::SceneNodeComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::LookAtComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::PerspectiveCameraComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::SceneMemberComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::SceneNodeComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::ViewportComponent<typename TEntityManager::Handle_type>>();
    }

}

