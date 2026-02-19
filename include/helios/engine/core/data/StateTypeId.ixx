
module;

#include <functional>

export module helios.engine.core.data.StateTypeId;

import helios.core.data.TypeIndexer;
import helios.core.types;

export namespace helios::engine::core::data {


    class StateTypeId {

        /**
         * @brief Tag type for the TypeIndexer domain.
         */
        struct helios_engine_core_data_StateTypes{};

        using StateType = helios_engine_core_data_StateTypes;

        /**
         * @brief The underlying ID value.
         */
        size_t id_{0};

        /**
         * @brief Private constructor for internal use.
         *
         * @param id The type ID value.
         */
        explicit StateTypeId(const size_t id) : id_(id) {}

    public:

        /**
         * @brief Constructs an uninitialized StateTypeId.
         *
         * @param helios::core::types::no_init_t Tag to indicate no initialization.
         */
        explicit StateTypeId(helios::core::types::no_init_t) {}

        /**
         * @brief Returns the underlying ID value.
         *
         * @return The numeric type ID, suitable for use as an array index.
         */
        [[nodiscard]] size_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Gets the StateTypeId for a specific component type.
         *
         * @details Uses TypeIndexer to generate a unique, monotonically increasing ID
         * for each component type. The ID is generated once per type and cached.
         *
         * @tparam T The component type. Must be a concrete type (not abstract).
         *
         * @return The unique StateTypeId for type T.
         */
        template <typename T>
        [[nodiscard]] static StateTypeId id() {
            static const size_t tid = helios::core::data::TypeIndexer<StateType>::typeIndex<T>();
            return StateTypeId(tid);
        }

        friend constexpr bool operator==(StateTypeId, StateTypeId) noexcept = default;
    };


}


/**
 * @brief std::hash specialization for StateTypeId.
 *
 * @details Enables use of StateTypeId as a key in unordered containers.
 */
template<>
struct std::hash<helios::engine::core::data::StateTypeId> {
   std::size_t operator()(const helios::engine::core::data::StateTypeId& id) const noexcept {
        return id.value();
    }

};