/**
 * @file registry.ixx
 * @brief Component registration for all engine modules.
 */
module;

export module helios.engine.modules.registry;

import helios.gameplay.ai.registry;
import helios.rendering.registry;
import helios.engine.modules.spatial.registry;
import helios.engine.modules.ui.registry;
import helios.scene.registry;
import helios.engine.modules.physics.registry;
import helios.gameplay.effects.registry;

export namespace helios::engine::modules {

    /**
     * @brief Registers all module components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::gameplay::ai::registerComponents<TEntityManager>();
        helios::engine::modules::spatial::registerComponents<TEntityManager>();
        helios::engine::modules::ui::registerComponents<TEntityManager>();
        helios::engine::modules::physics::registerComponents<TEntityManager>();
        helios::gameplay::effects::registerComponents<TEntityManager>();
    }

}

