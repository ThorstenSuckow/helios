/**
 * @file ManagerTypeId.ixx
 * @brief Unique type identifier for engine resources.
 */
module;

#include <functional>

export module helios.engine.runtime.world.types.ManagerTypeId;

import helios.core.TypeIndexer;
import helios.core.types;

export namespace helios::engine::runtime::world::types {

    /**
     * @brief Unique type identifier for engine resources.
     *
     * @details ManagerTypeId assigns a unique, monotonically increasing
     * integer ID to each resource type at compile time. Used by
     * ResourceRegistry as an index into the `fastAccess_` array for
     * O(1) type-based resource lookup.
     *
     * All resource categories (Managers, CommandBuffers, CommandHandlers)
     * share a single index space.
     *
     * @see ResourceRegistry
     * @see TypeIndexer
     * @see ComponentTypeId
     * @see CommandTypeId
     */
    class ManagerTypeId {

        /**
         * @brief Tag type for the TypeIndexer domain.
         */
        struct helios_engine_common_tag_ResourceTypes{};

        using ResourceType = helios_engine_common_tag_ResourceTypes;

        /**
         * @brief The underlying ID value.
         */
        size_t id_{0};


    public:


        /**
         * @brief Constructs a ManagerTypeId with a specific value.
         *
         * @param id The type ID value.
         */
        explicit ManagerTypeId(const size_t id) : id_(id) {}


        /**
         * @brief Constructs an uninitialized ManagerTypeId.
         *
         * @param no_init_t Tag to indicate no initialization.
         */
        explicit ManagerTypeId(helios::core::types::no_init_t) {}


        /**
         * @brief Returns the underlying ID value.
         *
         * @return The numeric type ID, suitable for use as an array index.
         */
        [[nodiscard]] size_t value() const noexcept {
            return id_;
        }


        /**
         * @brief Returns the ManagerTypeId for a specific type.
         *
         * @details Uses TypeIndexer to generate a unique ID per type.
         * The ID is generated once and cached.
         *
         * @tparam T The resource type.
         *
         * @return The unique ManagerTypeId for type T.
         */
        template <typename T>
        [[nodiscard]] static ManagerTypeId id() {
            static const size_t tid = helios::core::TypeIndexer<ResourceType>::typeIndex<T>();
            return ManagerTypeId(tid);
        }

        friend constexpr bool operator==(ManagerTypeId, ManagerTypeId) noexcept = default;
    };


}


/**
 * @brief Hash specialization for ManagerTypeId.
 */
template<>
struct std::hash<helios::engine::runtime::world::types::ManagerTypeId> {
   std::size_t operator()(const helios::engine::runtime::world::types::ManagerTypeId& id) const noexcept {
        return id.value();
    }

};