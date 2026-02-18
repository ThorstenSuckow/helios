/**
 * @file CombinedStateToIdMapPair.ixx
 * @brief Combined policy mapping two state types to IDs using 2D indexing.
 */
module;

#include <vector>
#include <span>
#include <helios/helios_config.h>
#include <cassert>
#include <bit>
#include <algorithm>
#include <iterator>
#include <bit>

export module helios.engine.state.CombinedStateToIdMapPair;

import helios.engine.state.StateToIdMap;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.core.data.ViewportId;
import helios.core.types;

export namespace helios::engine::state {

    using namespace helios::engine::core::data;
    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief Maps combined state pairs directly to ID lists.
     *
     * @details Unlike StateToIdMapPair which stores IDs per state type
     * and merges them at lookup time, this class uses a 2D matrix indexed
     * by both state bit positions. This allows associating IDs with
     * specific state combinations rather than individual states.
     *
     * The right state (RState) supports an "undefined" value (0) which
     * maps to index 0, allowing fallback behavior when only the left
     * state is defined.
     *
     * @tparam LState The left/primary state type.
     * @tparam RState The right/secondary state type.
     * @tparam TId The ID type to associate with state combinations.
     *
     * @see StateToIdMapPair
     */
    template<typename LState, typename RState, typename TId>
    class CombinedStateToIdMapPair {

        /**
         * @brief 2D matrix of ID lists indexed by [LState][RState].
         */
        std::vector<std::vector<std::vector<TId>>> states_;

        /**
         * @brief Empty vector returned for invalid lookups.
         */
        const std::vector<TId> empty_;

        /**
         * @brief Whether the map is frozen.
         */
        bool frozen_ = false;

        /**
         * @brief Computes the index for a left state value.
         *
         * @param stateLft The left state (must be non-zero, power of 2).
         *
         * @return The bit position index.
         */
        [[nodiscard]] static constexpr size_t indexOf(const LState stateLft) {
            const auto stL = static_cast<size_t>(std::to_underlying(stateLft));
            assert(stL != 0 && "LState must be defined");

            assert((std::to_underlying(stateLft) & (std::to_underlying(stateLft) -1)) == 0 && "LState must be a power of 2");

            return std::countr_zero(std::to_underlying(stateLft));
        }

        /**
         * @brief Computes the index for a right state value.
         *
         * @details Supports undefined (0) state which maps to index 0.
         * Non-zero states map to their bit position + 1.
         *
         * @param stateRgt The right state (may be 0, otherwise power of 2).
         *
         * @return The index (0 for undefined, bit position + 1 otherwise).
         */
        [[nodiscard]] static constexpr size_t indexOf(const RState stateRgt) {

            if (std::to_underlying(stateRgt) != 0 ) {
                assert((std::to_underlying(stateRgt) & (std::to_underlying(stateRgt) -1)) == 0 && "RState must be a power of 2");
                // undefined should map to 0, so 1 << 0 (bit count = 0) must map to 1
                // (countr_zero counts the number of zeroes beginning at the lsb)
                return std::countr_zero(std::to_underlying(stateRgt)) + 1;
            }

            return 0;
        }

    public:

        /**
         * @brief Adds an ID for a specific state combination.
         *
         * @param stateLft The left state (must be non-zero).
         * @param stateRgt The right state (may be 0 for fallback).
         * @param id The ID to associate with this combination.
         *
         * @return Reference to this map for chaining.
         */
        CombinedStateToIdMapPair& add(const LState stateLft, const RState stateRgt, const TId id) noexcept {

            assert(!frozen_ && "Cannot add to a frozen map");

            if (frozen_) {
                return *this;
            }

            const auto stL = indexOf(stateLft);
            const auto stR = indexOf(stateRgt);

            if (states_.size() <= stL) {
                states_.resize(stL + 1);
            }
            if (states_[stL].size() <= stR) {
                states_[stL].resize(stR + 1);
            }

            states_[stL][stR].push_back(id);

            return *this;
        }

        /**
         * @brief Sorts and deduplicates all ID lists.
         */
        void finalize() {
            for (auto& statesL : states_) {
                for (auto& statesR : statesL) {
                    std::sort(statesR.begin(), statesR.end());
                    auto [first, last] = std::ranges::unique(statesR);
                    statesR.erase(first, last);
                }
            }
        }

        /**
         * @brief Finalizes and prevents further modifications.
         */
        void freeze() {
            finalize();
            frozen_ = true;
        }

        /**
         * @brief Checks if the map is frozen.
         *
         * @return True if frozen.
         */
        [[nodiscard]] bool isFrozen() const noexcept {
            return frozen_;
        }

        /**
         * @brief Returns IDs for a state combination.
         *
         * @details If the specific combination has no IDs, falls back to
         * the IDs registered for the left state with undefined right state.
         *
         * @param stateLft The left state to query.
         * @param stateRgt The right state to query.
         *
         * @return Span of IDs for the state combination.
         */
        [[nodiscard]] std::span<const TId> ids(const LState stateLft, const RState stateRgt) const noexcept {

            assert(frozen_ && "Cannot return from a non-frozen map");

            const auto stL = indexOf(stateLft);
            const auto stR = indexOf(stateRgt);

            if (states_.size() <= stL) {
                return empty_;
            }

            if (states_[stL].size() <= stR  || states_[stL][stR].empty()) {
                if (!states_[stL].empty()) {
                    return states_[stL][0];
                }
                return empty_;
            }

            return states_[stL][stR];
        }

    };


}