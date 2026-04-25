/**
 * @file ComponentTypeId.ixx
 * @brief Compile-time type identifier for component types.
 *
 * @details Provides a unique, compile-time generated ID for each component type,
 * enabling O(1) direct indexing into the component storage vector within GameObject.
 */
module;

#include <functional>
#include <cstddef>

export module helios.ecs.types.ComponentTypeId;

import helios.core.TypeIndexer;
import helios.core.types.TypeDefs;

export namespace helios::ecs::types {

    /**
     * @brief Unique type identifier for component types, scoped per domain.
     *
     * @details ComponentTypeId assigns a unique, monotonically increasing integer ID
     * to each component type at compile time. This ID is used as an index into the
     * component storage vector within EntityManager, enabling O(1) access.
     *
     * ## Domain Scoping
     *
     * The class is templated on `THandle` so that each domain (identified by its
     * `EntityHandle<TDomainTag>` specialization) has its own independent type-ID
     * counter via `TypeIndexer<THandle>`. This means the same component type `T`
     * can receive different IDs in different domains.
     *
     * **Usage:**
     * ```cpp
     * // Get the ID for a component type in the game domain
     * auto id = ComponentTypeId<GameHandle>::id<Move2DComponent>();
     *
     * // Use the value as an index
     * components_[id.value()] = std::move(component);
     * ```
     *
     * **Thread Safety:** The static ID generation is thread-safe due to C++11 static
     * initialization guarantees.
     *
     * @tparam THandle The concrete `EntityHandle<TDomainTag>` specialization
     *                 scoping this type-ID counter.
     *
     * @see TypeIndexer
     * @see EntityManager
     */
    template<typename THandle>
    class ComponentTypeId {

        /**
         * @brief The underlying ID value.
         */
        size_t id_{0};

    public:

        /**
         * @brief Constructs a ComponentTypeId with a specific ID value.
         *
         * @details This constructor initializes a ComponentTypeId object with a given
         * ID, representing a unique identifier for a component type. The ID is used
         * internally for indexing and retrieving components from the storage system.
         *
         * @param id The unique identifier assigned to the component type.
         * This ID must be a valid non-negative value.
         */
        explicit ComponentTypeId(const size_t id) : id_(id) {}

        /**
         * @brief Constructs an uninitialized ComponentTypeId.
         *
         * @param no_init_t Tag to indicate no initialization.
         */
        explicit ComponentTypeId(helios::core::types::no_init_t) {}

        /**
         * @brief Returns the underlying ID value.
         *
         * @return The numeric type ID, suitable for use as an array index.
         */
        [[nodiscard]] size_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Gets the ComponentTypeId for a specific component type.
         *
         * @details Uses TypeIndexer to generate a unique, monotonically increasing ID
         * for each component type. The ID is generated once per type and cached.
         *
         * @tparam T The component type. Must be a concrete type (not abstract).
         *
         * @return The unique ComponentTypeId for type T.
         */
        template <typename T>
        [[nodiscard]] static ComponentTypeId<THandle> id() {
            static const size_t tid = helios::core::TypeIndexer<THandle>::template typeIndex<T>();
            return ComponentTypeId(tid);
        }

        friend constexpr bool operator==(ComponentTypeId, ComponentTypeId) noexcept = default;
    };


}


/**
 * @brief std::hash specialization for ComponentTypeId.
 *
 * @details Enables use of ComponentTypeId as a key in unordered containers.
 */
template<typename THandle>
struct std::hash<helios::ecs::types::ComponentTypeId<THandle>> {
   std::size_t operator()(const helios::ecs::types::ComponentTypeId<THandle>& id) const noexcept {
        return id.value();
    }

};