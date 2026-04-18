/**
 * @file CommandBufferTypeId.ixx
 * @brief Unique type identifier for command buffers.
 */
module;

#include <functional>

export module helios.engine.runtime.messaging.command.types.CommandBufferTypeId;

import helios.core.TypeIndexer;
import helios.core.types;

export namespace helios::engine::runtime::messaging::command::types {

    /**
     * @brief Unique type identifier for command buffer types.
     *
     * @details CommandBufferTypeId assigns a unique, monotonically increasing
     * integer ID to each command buffer type at compile time. The ID is generated
     * once per type via TypeIndexer and cached in a function-local static.
     *
     * Used by CommandBufferRegistry as an index for O(1) type-based lookup.
     *
     * @see CommandBufferRegistry
     * @see TypeIndexer
     * @see ResourceTypeId
     */
    class CommandBufferTypeId {

        /**
         * @brief Tag type for the TypeIndexer domain.
         */
        struct helios_engine_common_tag_CommandTypes{};

        using ComponentType = helios_engine_common_tag_CommandTypes;

        /**
         * @brief The underlying ID value.
         */
        size_t id_{0};


    public:


        /**
         * @brief Constructs a CommandBufferTypeId with a specific value.
         *
         * @param id The type ID value.
         */
        explicit CommandBufferTypeId(const size_t id) : id_(id) {}


        /**
         * @brief Constructs an uninitialized CommandBufferTypeId.
         *
         * @param no_init_t Tag to indicate no initialization.
         */
        explicit CommandBufferTypeId(helios::core::types::no_init_t) {}


        /**
         * @brief Returns the underlying ID value.
         *
         * @return The numeric type ID.
         */
        [[nodiscard]] size_t value() const noexcept {
            return id_;
        }


        /**
         * @brief Returns the CommandBufferTypeId for a specific command type.
         *
         * @details Uses TypeIndexer to generate a unique ID per type.
         * The ID is generated once and cached.
         *
         * @tparam T The command type.
         *
         * @return The unique CommandBufferTypeId for type T.
         */
        template <typename T>
        [[nodiscard]] static CommandBufferTypeId id() {
            static const size_t tid = helios::core::TypeIndexer<ComponentType>::typeIndex<T>();
            return CommandBufferTypeId(tid);
        }

        friend constexpr bool operator==(CommandBufferTypeId, CommandBufferTypeId) noexcept = default;
    };


}


/**
 * @brief Hash specialization for CommandBufferTypeId.
 */
template<>
struct std::hash<helios::engine::runtime::messaging::command::types::CommandBufferTypeId> {
   std::size_t operator()(const helios::engine::runtime::messaging::command::types::CommandBufferTypeId& id) const noexcept {
        return id.value();
    }

};