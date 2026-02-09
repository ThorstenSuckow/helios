/**
 * @file ComponentTypeId.ixx
 * @brief Compile-time type identifier for component types.
 *
 * @details Provides a unique, compile-time generated ID for each component type,
 * enabling O(1) direct indexing into the component storage vector within GameObject.
 */
module;

#include <functional>

export module helios.engine.core.data.ComponentTypeId;

import helios.core.data.TypeIndexer;
import helios.core.types;

export namespace helios::engine::core::data {

    /**
     * @brief Unique type identifier for component types.
     *
     * @details ComponentTypeId assigns a unique, monotonically increasing integer ID
     * to each component type at compile time. This ID is used as an index into the
     * component storage vector within GameObject, enabling O(1) access.
     *
     * **Usage:**
     * ```cpp
     * // Get the ID for a component type
     * auto id = ComponentTypeId::id<Move2DComponent>();
     *
     * // Use the value as an index
     * components_[id.value()] = std::move(component);
     * ```
     *
     * **Thread Safety:** The static ID generation is thread-safe due to C++11 static
     * initialization guarantees.
     *
     * @see TypeIndexer
     * @see GameObject
     */
    class ComponentTypeId {

        /**
         * @brief Tag type for the TypeIndexer domain.
         */
        struct helios_engine_core_data_ComponentTypes{};

        using ComponentType = helios_engine_core_data_ComponentTypes;

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
        [[nodiscard]] static ComponentTypeId id() {
            static const size_t tid = helios::core::data::TypeIndexer<ComponentType>::typeIndex<T>();
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
template<>
struct std::hash<helios::engine::core::data::ComponentTypeId> {
   std::size_t operator()(const helios::engine::core::data::ComponentTypeId& id) const noexcept {
        return id.value();
    }

};