/**
 * @file RuntimeHandle.ixx
 * @brief Lightweight handle pairing a strong identifier with a runtime index.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.core.data.RuntimeHandle;

import helios.core.data.StrongId;
import helios.core.types;


using namespace helios::core::types;
export namespace helios::core::data {

    /**
     * @brief A lightweight handle that pairs a stable StrongIdentifier with a dense RuntimeId.
     *
     * @details `RuntimeHandle` bridges the gap between content-authored, hash-based
     * identifiers (`StrongId`) and fast, contiguous runtime indices used by pools
     * and registries. The handle is a plain value type suitable for storage in
     * components, containers, and command buffers.
     *
     * Equality is defined over both fields; two handles are equal only when both
     * their `strongId` and `runtimeId` match.
     *
     * A `std::hash` specialization is provided so that `RuntimeHandle` can be used
     * as a key in unordered containers.
     *
     * @tparam StrongIdentifier The compile-time identifier type (typically a `StrongId<Tag>`).
     * @tparam RuntimeId The dense runtime index type (typically `uint32_t`).
     *
     * @see StrongId
     * @see DenseRuntimeHandleRegistry
     */
    template<typename StrongIdentifier, typename RuntimeId>
    struct RuntimeHandle {

        /**
         * @brief The stable, content-authored identifier.
         */
        StrongIdentifier strongId{};

        /**
         * @brief The dense runtime index assigned by a registry or pool.
         */
        RuntimeId runtimeId{};

        /**
         * @brief Defaulted equality comparison over both fields.
         */
        friend bool operator==(const RuntimeHandle&, const RuntimeHandle&) = default;


    };


}

/**
 * @brief Hash specialization for RuntimeHandle.
 *
 * @details Packs `strongId` and `runtimeId` into a single 64-bit value
 * before hashing, assuming both underlying types are `uint32_t`.
 *
 * @tparam StrongIdentifier The compile-time identifier type.
 * @tparam RuntimeId The runtime index type.
 */
template<typename StrongIdentifier, typename RuntimeId>
struct std::hash<helios::core::data::RuntimeHandle<StrongIdentifier, RuntimeId>> {
    std::size_t operator()(const helios::core::data::RuntimeHandle<StrongIdentifier, RuntimeId>& handle) const noexcept {

        static_assert(std::is_same_v<helios::core::types::StrongId_t, uint32_t>);
        static_assert(std::is_same_v<RuntimeId, uint32_t>);

        const uint64_t packed = (static_cast<uint64_t>(handle.strongId.value()) << 32) |
                                static_cast<uint64_t>(handle.runtimeId);

        return std::hash<uint64_t>{}(packed);
    }
};