/**
 * @file registry.ixx
 * @brief Component registration for all engine modules.
 */
module;

export module helios.registry;

import helios.ecs.registry;
import helios.gameplay.registry;
import helios.rendering.registry;
import helios.spatial.registry;
import helios.ui.registry;
import helios.scene.registry;
import helios.physics.registry;

export namespace helios {

    /**
     * @brief Registers all module components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::ecs::registerComponents<TEntityManager>();
        helios::gameplay::registerComponents<TEntityManager>();
        helios::rendering::registerComponents<TEntityManager>();
        helios::spatial::registerComponents<TEntityManager>();
        helios::ui::registerComponents<TEntityManager>();
        helios::scene::registerComponents<TEntityManager>();
        helios::physics::registerComponents<TEntityManager>();
    }

}
