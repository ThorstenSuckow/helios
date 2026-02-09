/**
 * @file ComponentOpsRegistry.ixx
 * @brief Global registry mapping ComponentTypeId to ComponentOps.
 */
module;

#include <vector>

export module helios.engine.ecs.ComponentOpsRegistry;

import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.ComponentOps;
import helios.engine.ecs.Traits;
import helios.engine.core.data.ComponentTypeId;


export namespace helios::engine::ecs {

    /**
     * @brief Global registry for component lifecycle function pointers.
     *
     * @details `ComponentOpsRegistry` provides O(1) lookup of `ComponentOps`
     * by `ComponentTypeId`. It uses a type-indexed vector where the index
     * corresponds to the component type's unique identifier.
     *
     * ## Usage
     *
     * ```cpp
     * // Registration (done by ComponentReflector)
     * ComponentOpsRegistry::setOps(typeId, ops);
     *
     * // Lookup at runtime
     * const auto& ops = ComponentOpsRegistry::ops(typeId);
     * if (ops.onAcquire) {
     *     ops.onAcquire(rawComponent);
     * }
     * ```
     *
     * @note Not thread-safe. All registration must complete before concurrent access.
     *
     * @see ComponentOps
     * @see ComponentReflector
     */
    class ComponentOpsRegistry {

        /**
         * @brief Type-indexed storage for ComponentOps.
         */
        inline static std::vector<ComponentOps> operations_;

        /**
         * @brief Empty ops returned for unregistered types.
         */
        inline static constexpr ComponentOps emptyOps_{};

    public:

        /**
         * @brief Registers ComponentOps for a component type.
         *
         * @details Auto-resizes the internal vector if necessary.
         *
         * @param typeId The unique identifier for the component type.
         * @param ops The lifecycle function pointers to register.
         */
        static void setOps(const helios::engine::core::data::ComponentTypeId typeId, const ComponentOps& ops) {

            if (typeId.value() >= operations_.size()) {
                operations_.resize(typeId.value() + 1);
            }

            operations_[typeId.value()] = ops;
        }

        /**
         * @brief Retrieves ComponentOps for a component type.
         *
         * @param typeId The unique identifier for the component type.
         *
         * @return Reference to the registered ComponentOps, or empty ops if not registered.
         */
        static const ComponentOps& ops(const helios::engine::core::data::ComponentTypeId typeId) {

            if (typeId.value() >= operations_.size()) {
                return emptyOps_;
            }

            return operations_[typeId.value()];
        }

    };

}