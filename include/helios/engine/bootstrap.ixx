/**
 * @file Bootstrap.ixx
 * @brief Central registration of all component types for reflection.
 */
module;

export module helios.engine.Bootstrap;

import helios.engine.mechanics.registry;
import helios.engine.modules.registry;

export namespace helios::engine::bootstrap {

    /**
     * @brief Registers all component types with the ComponentReflector.
     *
     * @details This function must be called during engine initialization to
     * enable runtime reflection features such as cloning, lifecycle callbacks
     * (onAcquire, onRelease, onRemove), and enable/disable toggles.
     *
     * ## Usage
     *
     * ```cpp
     * // Call once during engine startup
     * helios::engine::bootstrap::registerAllComponents();
     * ```
     *
     * @note New component types must be added to the respective module's
     *       registry.ixx file to participate in the reflection system.
     *
     * @see ComponentReflector
     * @see ComponentOpsRegistry
     */
    inline void registerAllComponents() {

        static bool done = false;
        if (done) return;
        done = true;

        helios::engine::mechanics::registerComponents();
        helios::engine::modules::registerComponents();

    }

}

