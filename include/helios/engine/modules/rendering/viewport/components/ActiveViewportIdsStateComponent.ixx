/**
 * @file ActiveViewportIdsStateComponent.ixx
 * @brief Component storing active viewport IDs for state-based rendering.
 */
module;

#include <vector>
#include <span>

export module helios.engine.modules.rendering.viewport.components.ActiveViewportIdsStateComponent;

import helios.engine.ecs.EntityHandle;
import helios.engine.core.data;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

export namespace helios::engine::modules::rendering::viewport::components {

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::core::data;

    /**
     * @brief Stores the currently active viewport IDs based on game/match state.
     *
     * @details This component maintains a list of ViewportIds that should be
     * rendered for the current game and match state combination. It is updated
     * by StateToViewportPolicyUpdateSystem each frame based on the configured
     * StateToViewportPolicy.
     */
    class ActiveViewportIdsStateComponent {

        /**
         * @brief Currently active viewport identifiers.
         */
        mutable std::vector<ViewportId> viewportIds_;


    public:

        /**
         * @brief Replaces the current viewport IDs with the provided list.
         *
         * @param viewportIds The new list of active viewport IDs.
         */
        void addViewportIds(const std::vector<ViewportId>& viewportIds) {
            viewportIds_ = viewportIds;
        }

        /**
         * @brief Returns a span over all active viewport IDs.
         *
         * @return Read-only span of viewport identifiers.
         */
        [[nodiscard]] std::span<const ViewportId> viewportIds() const noexcept {
            return viewportIds_;
        }

        /**
         * @brief Clears all active viewport IDs.
         */
        void clear() const noexcept {
            viewportIds_.clear();
        }

    };

}