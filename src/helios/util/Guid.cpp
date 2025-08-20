module;

#include <atomic>

module helios.util.Guid;

namespace helios::util {
    Guid::Guid(const uint64_t value) noexcept : val(value) {}

    Guid Guid::generate() noexcept {
        static std::atomic<uint64_t> counter{1};
        return Guid(counter.fetch_add(1, std::memory_order_relaxed));
    }
}
