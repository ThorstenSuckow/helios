/**
 * @file Guid.ixx
 * @brief Lightweight GUID (globally unique identifier) implementation.
 */
module;

#include <atomic>
#include <cstdint>


export module helios.util.Guid;


export namespace helios::util {

    struct no_init_t{};

    inline constexpr no_init_t no_init;

    /**
     * @brief Representative of a Globally Unique Identifier.
     *
     * Generating GUIDs with this class is considered thread-safe.
     */
    class Guid final {
    private:
        explicit Guid(uint64_t value) noexcept : value_(value) {}
        uint64_t value_{};

    public:

        /**
         * @brief Unsafe Guid initializer for (local) var initialization.
         */
        explicit Guid(no_init_t) {};

        /**
         * @brief Compares two Guid instances for equality.
         *
         * @param guid The Guid to compare against.
         *
         * @return `true` if both Guids have the same underlying value, `false` otherwise.
         */
        constexpr bool operator==(const Guid& guid) const = default;

        /**
         * @brief Compares two Guid instances for inequality.
         *
         * @param guid The Guid to compare against.
         *
         * @return `true` if the Guids have different underlying values, `false` otherwise.
         */
        constexpr bool operator!=(const Guid& guid) const = default;

        /**
         * @brief Less-than comparison operator, enabling ordering of Guid instances.
         *
         * @param guid The Guid to compare against.
         *
         * @return `true` if this Guid's value is less than the other, `false` otherwise.
         */
        constexpr bool operator<(const Guid& guid) const noexcept {
            return value_ < guid.value();
        }

        /**
         * @brief Greater-than comparison operator, enabling ordering of Guid instances.
         *
         * @param guid The Guid to compare against.
         *
         * @return `true` if this Guid's value is greater than the other, `false` otherwise.
         */
        constexpr bool operator>(const Guid& guid) const noexcept {
            return value_ > guid.value();
        }


        /**
         * @brief Generates a new Guid.
         *
         * This function produces a new, unique Guid value. It is safe to call from
         * multiple threads.
         *
         * @return A newly generated `Guid` instance.
         */
        static Guid generate() noexcept {
            static std::atomic<uint64_t> counter{1};
            return Guid(counter.fetch_add(1, std::memory_order_relaxed));
        }

        /**
         * @brief Returns the raw 64-bit value of this Guid.
         *
         * @return The underlying uint64_t value representing this Guid.
         */
        [[nodiscard]] constexpr uint64_t value() const noexcept {
            return value_;
        }

        /**
         * @brief Calculates a hash fot this GUID.
         *
         * The hash simply defaults to the value_ of this Guid.
         *
         * @return The hash value for this Guid.
         */
        [[nodiscard]] std::size_t hash() const noexcept {
            return std::hash<uint64_t>{}(value_);
        }

    };

} // namespace helios::util


template<>
struct std::hash<helios::util::Guid> {
    std::size_t operator()(const helios::util::Guid& guid) const noexcept {
        return guid.hash();
    }
};