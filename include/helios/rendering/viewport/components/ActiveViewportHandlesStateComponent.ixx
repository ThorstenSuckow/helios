/**
 * @file ActiveViewportIdsStateComponent.ixx
 * @brief Component storing active viewport IDs for state-based rendering.
 */
module;

#include <vector>
#include <span>

export module helios.rendering.viewport.components.ActiveViewportHandlesStateComponent;

import helios.ecs.types.EntityHandle;
import helios.rendering.viewport.types.ViewportHandle;

import helios.gameplay.gamestate.types;
import helios.gameplay.matchstate.types;

using namespace helios::gameplay::gamestate::types;
using namespace helios::gameplay::matchstate::types;
using namespace helios::rendering::viewport::types;
export namespace helios::rendering::viewport::components {

    /**
     * @brief Stores the currently active viewport IDs based on game/match state.
     *
     * @details This component maintains a list of ViewportIds that should be
     * rendered for the current game and match state combination. It is updated
     * by StateToViewportPolicyUpdateSystem each frame based on the configured
     * StateToViewportPolicy.
     */
    template<typename THandle>
    class ActiveViewportHandlesStateComponent {

        /**
         * @brief Currently active viewport identifiers.
         */
        mutable std::vector<ViewportHandle> viewportHandles_;


    public:

        /**
         * @brief Replaces the current viewport handles with the provided list.
         *
         * @param viewportIds The new list of active viewport handles.
         */
        void setViewportHandles(std::span<const ViewportHandle>& viewportHandles) {
            viewportHandles_.assign(viewportHandles.begin(), viewportHandles.end());
        }

        /**
         * @brief Returns a span over all active viewport IDs.
         *
         * @return Read-only span of viewport handles.
         */
        [[nodiscard]] std::span<const ViewportHandle> viewportHandles() const noexcept {
            return viewportHandles_;
        }

        /**
         * @brief Returns true if the ViewportId is contained in this component, otherwise false.
         *
         * @param viewportId
         * @return true if the ViewportId is currently active, false otherwise.
         */
        [[nodiscard]] bool has(const ViewportHandle viewportHandle) const noexcept {
            for (const auto& handle : viewportHandles_) {
                if (handle == viewportHandle) {
                    return true;
                }
            }

            return false;
        }

        /**
         * @brief Clears all active viewport IDs.
         */
        void clear() const noexcept {
            viewportHandles_.clear();
        }

    };

}