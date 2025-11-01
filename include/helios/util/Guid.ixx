module;

#include <atomic>
#include <cstdint>


export module helios.util.Guid;


export namespace helios::util {

    /**
     * @brief Representative of a Globally Unique Identifier.
     *
     * Generating guids with this class is considered thread-safe.
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
         * @return Guid an immutable Guid-instance
         */
        static Guid generate() noexcept;

        /**
         * @brief Returns the raw value of this Guid.
         *
         * @return uint64_t
         */
        [[nodiscard]] constexpr uint64_t value() const noexcept {
            return value_;
        }

    };

}
