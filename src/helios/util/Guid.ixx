module;

#include <atomic>
#include <cstdint>
#include <memory>


export module helios.util.Guid;


export namespace helios::util {

    class Guid {
    private:
        explicit Guid(uint64_t value) noexcept;
        uint64_t val;

    public:
        constexpr uint64_t value() const noexcept { return this->val; }

        static Guid generate() noexcept;

        constexpr bool operator==(const Guid& guid) const = default;

        constexpr bool operator!=(const Guid& guid) const = default;
    };

}
