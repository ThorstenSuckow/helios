/**
 * @file ComponentOpsRegistry.ixx
 * @brief Global registry mapping ComponentTypeId to ComponentOps.
 */
module;

#include <vector>

export module helios.ecs.ComponentOpsRegistry;

import helios.ecs.types.EntityHandle;
import helios.ecs.types.ComponentOps;
import helios.ecs.types.ComponentTypeId;

using namespace helios::ecs::types;
export namespace helios::ecs {

    /**
     * @brief Global registry for component lifecycle function pointers.
     *
     * @details `ComponentOpsRegistry` provides O(1) lookup of `ComponentOps`
     * by `ComponentTypeId`. It uses a type-indexed vector where the index
     * corresponds to the component type's unique identifier.
     *
     * ## Domain Scoping
     *
     * The registry is templated on `THandle` so that each domain
     * (identified by its `EntityHandle<TDomainTag>` specialization) has its
     * own independent, `inline static` ops storage. This prevents lifecycle
     * hooks registered for one domain from interfering with another.
     *
     * ## Usage
     *
     * ```cpp
     * // Registration (done by ComponentReflector<THandle, TEM>)
     * ComponentOpsRegistry<GameHandle>::setOps(typeId, ops);
     *
     * // Lookup at runtime
     * const auto& ops = ComponentOpsRegistry<GameHandle>::ops(typeId);
     * if (ops.onAcquire) {
     *     ops.onAcquire(rawComponent);
     * }
     * ```
     *
     * @note Not thread-safe. All registration must complete before concurrent access.
     *
     * @tparam THandle The concrete `EntityHandle<TDomainTag>` specialization
     *                 scoping this registry instance.
     *
     * @see ComponentOps
     * @see ComponentReflector
     */
    template<typename THandle>
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

        using ComponentTypeId_type = ComponentTypeId<THandle>;


        /**
         * @brief Registers ComponentOps for a component type.
         *
         * @details Auto-resizes the internal vector if necessary.
         *
         * @param typeId The unique identifier for the component type.
         * @param ops The lifecycle function pointers to register.
         */
        static void setOps(const ComponentTypeId_type typeId, const ComponentOps& ops) {

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
        static const ComponentOps& ops(const ComponentTypeId_type typeId) {

            if (typeId.value() >= operations_.size()) {
                return emptyOps_;
            }

            return operations_[typeId.value()];
        }

    };

}