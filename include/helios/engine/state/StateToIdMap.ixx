/**
 * @file StateToIdMap.ixx
 * @brief Policy mapping states to IDs.
 */
module;

#include <vector>
#include <span>
#include <helios/helios_config.h>
#include <cassert>
#include <bit>
#include <algorithm>

export module helios.engine.state.StateToIdMap;


import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.core.data.ViewportId;
import helios.core.types;

export namespace helios::engine::state {

    using namespace helios::engine::core::data;
    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief Maps state enum values to lists of IDs.
     *
     * @details Provides a lookup table that associates each bit position
     * of a state enum with a list of IDs. States must be power-of-two
     * values (single bits). Supports finalization and freezing for
     * runtime optimization.
     *
     * @tparam TState The state enum type (e.g., GameState, MatchState).
     * @tparam TId The ID type to associate with states.
     */
    template<typename TState, typename TId>
    class StateToIdMap {

        /**
         * @brief ID lists indexed by state bit position.
         */
        std::vector<std::vector<TId>> states_;

        /**
         * @brief Empty vector returned for invalid lookups.
         */
        const std::vector<TId> empty_;

        /**
         * @brief Updates internal storage for a state-ID association.
         *
         * @param st The state value as size_t.
         * @param id The ID to associate.
         */
        void update(size_t st, const TId id) {
           
            size_t idx = 0;
            while (st > 0) {

                if ((st & 1) != 0) {
                    if (states_.size() <= idx) {
                        states_.resize(idx + 1);
                    }
                    if (std::ranges::find(states_[idx], id) == states_[idx].end()) {
                        states_[idx].push_back(id);
                    }
                }

                idx++;
                st >>= 1;
            }

        }

        /**
         * @brief Whether the map is frozen.
         */
        bool frozen_ = false;

    public:

        /**
         * @brief Default constructor.
         */
        StateToIdMap() {
            states_.reserve(STATE_TO_IDMAP_DEFAULT_CAPACITY);
        };

        /**
         * @brief Adds an ID for a given state.
         *
         * @param state The state to associate the ID with.
         * @param id The ID to add.
         *
         * @return Reference to this map for chaining.
         */
        StateToIdMap& add(TState state, TId id) noexcept {
            assert(!frozen_ && "Cannot add to a frozen map");

            const auto st = static_cast<size_t>(std::to_underlying(state));

            if (st == 0) {
                return *this;
            }

            update(st, id);

            return *this;
        }

        /**
         * @brief Sorts and deduplicates all ID lists.
         */
        void finalize() {
            for (auto& v : states_) {
                std::sort(v.begin(), v.end());
                auto [first, last] = std::ranges::unique(v);
                v.erase(first, last);
            }
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
         * @brief Finalizes and prevents further modifications.
         */
        void freeze() {
            finalize();
            states_.shrink_to_fit();
            frozen_ = true;
        }

        /**
         * @brief Returns IDs for a single state.
         *
         * @param state The state to query (must be a power of 2).
         *
         * @return Reference to the ID list for the state.
         */
        [[nodiscard]] const std::vector<TId>& ids(const TState state) const noexcept {

            if (std::to_underlying(state) == 0) {
                return empty_;
            }

            assert((std::to_underlying(state) & (std::to_underlying(state) -1)) == 0 && "State must be a power of 2");

            const auto stateIdx = static_cast<size_t>(std::countr_zero(std::to_underlying(state)));

            if (states_.size() <= stateIdx) {
                return empty_;
            }

            return states_[stateIdx];
        }


    };


}