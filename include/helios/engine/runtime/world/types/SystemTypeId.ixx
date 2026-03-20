/**
 * @file SystemTypeId.ixx
 * @brief Unique type identifier for system types.
 */
module;

#include <functional>

export module helios.engine.runtime.world.types.SystemTypeId;

import helios.core.data.TypeIndexer;
import helios.core.types;

export namespace helios::engine::runtime::world::types {

    /**
     * @brief Unique type identifier for system types.
     *
     * @details SystemTypeId assigns a unique, monotonically increasing
     * integer ID to each system type at compile time. Used by
     * SystemRegistry as an index for O(1) type-based system lookup.
     *
     * @see SystemRegistry
     * @see TypeIndexer
     * @see ResourceTypeId
     */
    class SystemTypeId {

        /**
         * @brief Tag type for the TypeIndexer domain.
         */
        struct helios_engine_common_tag_SystemTypes{};

        using SystemType = helios_engine_common_tag_SystemTypes;

        /**
         * @brief The underlying ID value.
         */
        size_t id_{0};


    public:


        /**
         * @brief Constructs a SystemTypeId with a specific value.
         *
         * @param id The type ID value.
         */
        explicit SystemTypeId(const size_t id) : id_(id) {}


        /**
         * @brief Constructs an uninitialized SystemTypeId.
         *
         * @param no_init_t Tag to indicate no initialization.
         */
        explicit SystemTypeId(helios::core::types::no_init_t) {}


        /**
         * @brief Returns the underlying ID value.
         *
         * @return The numeric type ID, suitable for use as an array index.
         */
        [[nodiscard]] size_t value() const noexcept {
            return id_;
        }


        /**
         * @brief Returns the SystemTypeId for a specific type.
         *
         * @details Uses TypeIndexer to generate a unique ID per type.
         * The ID is generated once and cached.
         *
         * @tparam T The resource type.
         *
         * @return The unique SystemTypeId for type T.
         */
        template <typename T>
        [[nodiscard]] static SystemTypeId id() {
            static const size_t tid = helios::core::data::TypeIndexer<SystemType>::typeIndex<T>();
            return SystemTypeId(tid);
        }

        friend constexpr bool operator==(SystemTypeId, SystemTypeId) noexcept = default;
    };


}


/**
 * @brief Hash specialization for SystemTypeId.
 */
template<>
struct std::hash<helios::engine::runtime::world::types::SystemTypeId> {
   std::size_t operator()(const helios::engine::runtime::world::types::SystemTypeId& id) const noexcept {
        return id.value();
    }

};