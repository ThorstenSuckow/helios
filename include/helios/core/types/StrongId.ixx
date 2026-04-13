/**
 * @file StrongId.ixx
 * @brief Strongly-typed identifier template.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.core.types.StrongId;

import helios.core.types.FuncDefs;
import helios.core.types.TypeDefs;

export namespace helios::core::types {

    /**
     * @brief A strongly-typed identifier using tag-based type safety.
     *
     * @details Provides compile-time type safety for identifiers by
     * associating each ID type with a unique tag struct. Supports
     * compile-time string hashing via FNV-1a algorithm.
     *
     * @tparam Tag Empty struct used to distinguish different ID types.
     */
    template<typename Tag>
    struct StrongId {

    private:

        /**
         * @brief The underlying ID value.
         */
        StrongId_t id_{};

    public:

        /**
         * @brief Constructs from a compile-time string.
         *
         * @details Uses FNV-1a hashing to convert the string to an ID.
         *
         * @param str The string to hash.
         */
        explicit constexpr StrongId(const std::string_view str) noexcept
            : StrongId(fnv1a_hash(str)) {}


        /**
         * @brief Constructs from a raw underlying value.
         *
         * @param id The raw ID value.
         */
        explicit constexpr StrongId(const StrongId_t id) noexcept
            : id_(id) {
        }

        /**
         * @brief Constructs an uninitialized ID.
         *
         * @param no_init_t Tag type indicating no initialization.
         */
         explicit constexpr StrongId(no_init_t) {};

        /**
         * @brief Default constructor creating an uninitialized ID.
         */
        explicit constexpr StrongId() : StrongId(no_init){};

        /**
         * @brief Returns true if the strong id is considered valid.
         *
         * @return True if the StrongId is considered valid, else false.
         */
        [[nodiscard]] bool isValid() const noexcept {;
            return id_ != 0;
        }

        /**
         * @brief Returns the underlying ID value.
         *
         * @return The raw underlying value.
         */
        [[nodiscard]] StrongId_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Checks if the ID is valid (non-zero).
         *
         * @return True if the ID is non-zero.
         */
        [[nodiscard]] explicit operator bool() const noexcept {
            return id_ != 0;
        }

        /**
         * @brief Equality comparison.
         */
        friend constexpr bool operator==(StrongId, StrongId) noexcept = default;

        /**
         * @brief Less-than comparison for ordering.
         *
         * @param other The other ID to compare.
         *
         * @return True if this ID is less than the other.
         */
        constexpr bool operator<(const StrongId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison for ordering.
         *
         * @param other The other ID to compare.
         *
         * @return True if this ID is greater than the other.
         */
        constexpr bool operator>(const StrongId& other) const noexcept {
            return id_ > other.id_;
        }

    };

}


template<typename Tag>
struct std::hash<helios::core::types::StrongId<Tag>> {
    helios::core::types::StrongId_t operator()(const helios::core::types::StrongId<Tag>& id) const noexcept {
        return id.value();
    }

};