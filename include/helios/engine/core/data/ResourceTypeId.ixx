/**
 * @file ResourceTypeId.ixx
 * @brief Unique type identifier for engine resources.
 */
module;

#include <functional>

export module helios.engine.core.data.ResourceTypeId;

import helios.core.data.TypeIndexer;
import helios.core.types;

export namespace helios::engine::core::data {

    /**
     * @brief Unique type identifier for engine resources.
     *
     * @details ResourceTypeId assigns a unique, monotonically increasing
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
    class ResourceTypeId {

        /**
         * @brief Tag type for the TypeIndexer domain.
         */
        struct helios_engine_core_data_ResourceTypes{};

        using ResourceType = helios_engine_core_data_ResourceTypes;

        /**
         * @brief The underlying ID value.
         */
        size_t id_{0};


    public:


        /**
         * @brief Constructs a ResourceTypeId with a specific value.
         *
         * @param id The type ID value.
         */
        explicit ResourceTypeId(const size_t id) : id_(id) {}


        /**
         * @brief Constructs an uninitialized ResourceTypeId.
         *
         * @param no_init_t Tag to indicate no initialization.
         */
        explicit ResourceTypeId(helios::core::types::no_init_t) {}


        /**
         * @brief Returns the underlying ID value.
         *
         * @return The numeric type ID, suitable for use as an array index.
         */
        [[nodiscard]] size_t value() const noexcept {
            return id_;
        }


        /**
         * @brief Returns the ResourceTypeId for a specific type.
         *
         * @details Uses TypeIndexer to generate a unique ID per type.
         * The ID is generated once and cached.
         *
         * @tparam T The resource type.
         *
         * @return The unique ResourceTypeId for type T.
         */
        template <typename T>
        [[nodiscard]] static ResourceTypeId id() {
            static const size_t tid = helios::core::data::TypeIndexer<ResourceType>::typeIndex<T>();
            return ResourceTypeId(tid);
        }

        friend constexpr bool operator==(ResourceTypeId, ResourceTypeId) noexcept = default;
    };


}


/**
 * @brief Hash specialization for ResourceTypeId.
 */
template<>
struct std::hash<helios::engine::core::data::ResourceTypeId> {
   std::size_t operator()(const helios::engine::core::data::ResourceTypeId& id) const noexcept {
        return id.value();
    }

};