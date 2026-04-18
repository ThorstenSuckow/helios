/**
 * @file registry.ixx
 * @brief Component registration for all engine modules.
 */
module;

export module helios.registry;

import helios.gameplay.ai.registry;
import helios.rendering.registry;
import helios.spatial.registry;
import helios.ui.registry;
import helios.scene.registry;
import helios.physics.registry;
import helios.gameplay.effects.registry;

export namespace helios {

    /**
     * @brief Registers all module components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::gameplay::ai::registerComponents<TEntityManager>();
        helios::spatial::registerComponents<TEntityManager>();
        helios::ui::registerComponents<TEntityManager>();
        helios::physics::registerComponents<TEntityManager>();
        helios::gameplay::effects::registerComponents<TEntityManager>();
    }

}

