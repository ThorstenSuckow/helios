/**
 * @file ScoreTypeId.ixx
 * @brief Strongly-typed identifier for score types.
 */
module;

#include <functional>

export module helios.engine.core.data.ScoreTypeId;

import helios.core.data.TypeIndexer;
import helios.core.types;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for score types.
     *
     * Uses TypeIndexer to generate unique, monotonically increasing IDs for each
     * score type at compile time. Suitable for use as array indices.
     */
    class ScoreTypeId {

        /**
         * @brief Tag type for the TypeIndexer domain.
         */
        struct helios_engine_core_data_ScoreTypes{};

        using ScoreType = helios_engine_core_data_ScoreTypes;

        /**
         * @brief The underlying ID value.
         */
        size_t id_{0};

        /**
         * @brief Private constructor for internal use.
         *
         * @param id The type ID value.
         */
        explicit ScoreTypeId(const size_t id) : id_(id) {}

    public:

        /**
         * @brief Constructs an uninitialized ScoreTypeId.
         *
         * @param helios::core::types::no_init_t Tag to indicate no initialization.
         */
        explicit ScoreTypeId(helios::core::types::no_init_t) {}

        /**
         * @brief Returns the underlying ID value.
         *
         * @return The numeric type ID, suitable for use as an array index.
         */
        [[nodiscard]] size_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Gets the ScoreTypeId for a specific component type.
         *
         * @details Uses TypeIndexer to generate a unique, monotonically increasing ID
         * for each component type. The ID is generated once per type and cached.
         *
         * @tparam T The component type. Must be a concrete type (not abstract).
         *
         * @return The unique ScoreTypeId for type T.
         */
        template <typename T>
        [[nodiscard]] static ScoreTypeId id() {
            static const size_t tid = helios::core::data::TypeIndexer<ScoreType>::typeIndex<T>();
            return ScoreTypeId(tid);
        }

        friend constexpr bool operator==(ScoreTypeId, ScoreTypeId) noexcept = default;
    };


}


/**
 * @brief std::hash specialization for ScoreTypeId.
 *
 * @details Enables use of ScoreTypeId as a key in unordered containers.
 */
template<>
struct std::hash<helios::engine::core::data::ScoreTypeId> {
   std::size_t operator()(const helios::engine::core::data::ScoreTypeId& id) const noexcept {
        return id.value();
    }

};