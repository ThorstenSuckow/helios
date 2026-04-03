/**
 * @file DenseRuntimeHandleRegistry.ixx
 * @brief Dense registry mapping strong identifiers to contiguous runtime indices.
 */
module;

#include <vector>
#include <helios/helios_config.h>
#include <cassert>
#include <optional>
#include <string>
#include <algorithm>

export module helios.core.container.DenseRuntimeHandleRegistry;

import helios.core.data.StrongId;
import helios.core.data.RuntimeHandle;
import helios.core.types;

using namespace helios::core::types;
using namespace helios::core::data;
export namespace helios::core::container {

    /**
     * @brief Maps strong identifiers to dense, contiguous runtime indices.
     *
     * @details `DenseRuntimeHandleRegistry` maintains a compact vector of
     * `StrongId` values. The position of each entry in the vector serves as
     * its `RuntimeId`, enabling O(1) access in downstream pools that are
     * indexed by the same `RuntimeId`.
     *
     * Registration is idempotent: calling `getOrCreate()` with an already
     * registered identifier returns the existing handle without duplication.
     *
     * ## Complexity
     *
     * - `has()` / `runtimeId()` — O(n) linear scan.
     * - `getOrCreate()` — O(n) lookup + amortised O(1) append on miss.
     *
     * ## Usage
     *
     * ```cpp
     * using MeshHandleRegistry =
     *     DenseRuntimeHandleRegistry<MeshId, MeshRuntimeId>;
     *
     * MeshHandleRegistry registry;
     * auto handle = registry.getOrCreate("player_mesh");
     * auto runtimeId = handle.runtimeId;          // dense index into MeshPool
     * bool exists = registry.has(MeshId{"player_mesh"});
     * ```
     *
     * @tparam StrongIdentifier A `StrongId<Tag>` type used as the stable key.
     * @tparam RuntimeId An unsigned integer type (typically `uint32_t`) used as
     *         the dense index.
     *
     * @see RuntimeHandle
     * @see StrongId
     */
    template<typename StrongIdentifier, typename RuntimeId>
    class DenseRuntimeHandleRegistry {

        /**
         * @brief Dense storage of registered strong-ID values.
         *
         * @details The index of each entry is its corresponding `RuntimeId`.
         */
        std::vector<StrongId_t> ids_;

    public:

        /**
         * @brief Constructs the registry with a reserved capacity.
         *
         * @param defaultCapacity Initial capacity hint for the internal vector.
         */
        explicit DenseRuntimeHandleRegistry(const size_t defaultCapacity = DEFAULT_RUNTIME_HANDLE_REGISTRY_CAPACITY) {
            ids_.reserve(defaultCapacity);
        }

        /**
         * @brief Checks whether a strong identifier is registered.
         *
         * @param strongId The identifier to look up.
         *
         * @return True if the identifier is present.
         */
        [[nodiscard]] bool has(StrongIdentifier strongId) const noexcept {
            return std::ranges::find(ids_, strongId.value()) != ids_.end();
        }

        /**
         * @brief Returns the runtime index for a registered identifier.
         *
         * @param strongId The identifier to look up.
         *
         * @return The associated `RuntimeId`, or `std::nullopt` if not registered.
         */
        [[nodiscard]] std::optional<RuntimeId> runtimeId(StrongIdentifier strongId) const noexcept {
            auto it = std::ranges::find(ids_, strongId.value());

            if (it == ids_.end()) {
                return std::nullopt;
            }

            return RuntimeId{static_cast<RuntimeId>(
                std::distance(ids_.begin(), it))
            };
        }

        /**
         * @brief Returns an existing handle or creates a new one from a string key.
         *
         * @details Constructs a `StrongIdentifier` from `strId` and delegates
         * to the `StrongIdentifier` overload.
         *
         * @param strId String key that is hashed into a `StrongIdentifier`.
         *
         * @return The existing or newly created handle.
         */
        RuntimeHandle<StrongIdentifier, RuntimeId> getOrCreate(std::string strId) {
            return getOrCreate(StrongIdentifier{std::move(strId)});
        }

        /**
         * @brief Returns an existing handle or registers a new entry.
         *
         * @details If the identifier is already registered, the existing
         * handle is returned. Otherwise a new entry is appended and a fresh
         * `RuntimeId` is assigned.
         *
         * @param strongId The identifier to register.
         *
         * @return The existing or newly created handle.
         *
         * @pre The registry must not have reached its reserved capacity.
         */
        RuntimeHandle<StrongIdentifier, RuntimeId> getOrCreate(const StrongIdentifier strongId) {
            const auto rid = runtimeId(strongId);
            if (rid.has_value()) {
                return RuntimeHandle<StrongIdentifier, RuntimeId>(strongId, *rid);
            }

            assert(ids_.size() < ids_.capacity() && "DenseRuntimeHandleRegistry capacity exceeded");
            ids_.push_back(strongId.value());
            return RuntimeHandle<StrongIdentifier, RuntimeId>{
                strongId,
                RuntimeId{static_cast<RuntimeId>(ids_.size() - 1)}
            };
        }
    };

}