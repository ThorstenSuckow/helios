/**
 * @file Guid.ixx
 * @brief Lightweight GUID (globally unique identifier) implementation.
 */
module;

#include <atomic>
#include <cstdint>


export module helios.util.Guid;


export namespace helios::util {

    /**
     * @brief Representative of a Globally Unique Identifier.
     *
     * Generating GUIDs with this class is considered thread-safe.
     */
    class Guid final {
    private:
        explicit Guid(uint64_t value) noexcept;
        uint64_t value_;

    public:

        constexpr bool operator==(const Guid& guid) const = default;
        constexpr bool operator!=(const Guid& guid) const = default;

        /**
         * @brief Generates a new Guid.
         *
         * This function produces a new, unique Guid value. It is safe to call from
         * multiple threads.
         *
         * @return A newly generated `Guid` instance.
         */
        static Guid generate() noexcept;

        /**
         * @brief Returns the raw 64-bit value of this Guid.
         *
         * @return The underlying uint64_t value representing this Guid.
         */
        [[nodiscard]] constexpr uint64_t value() const noexcept {
            return value_;
        }

    };

} // namespace helios::util
