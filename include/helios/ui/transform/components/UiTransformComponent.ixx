/**
 * @file UiTransformComponent.ixx
 * @brief Component for viewport-relative UI positioning.
 */
module;

#include <cmath>

export module helios.ui.transform.components.UiTransformComponent;



import helios.ui.layout.Anchor;

import helios.ecs.types.ComponentTypeId;
import helios.engine.common.types;

import helios.math;

import helios.core;
import helios.core;
import helios.rendering.viewport.types.ViewportHandle;

using namespace helios::rendering::viewport::types;
export namespace helios::ui::transform::components {


    /**
     * @brief Component for viewport-relative UI positioning.
     *
     * Stores positioning parameters for UI elements including anchor point,
     * pivot point, offsets, and the target viewport. Used by UiTransformSystem
     * to compute screen positions.
     */
    template<typename THandle>
    class UiTransformComponent{

        /**
         * @brief Anchor point relative to the viewport.
         */
        helios::ui::layout::Anchor anchor_{helios::ui::layout::Anchor::Center};

        /**
         * @brief Pivot point of this element for positioning calculations.
         */
        helios::ui::layout::Anchor pivot_{helios::ui::layout::Anchor::Center};

        /**
         * @brief Handle of the viewport this element is positioned relative to.
         */
        ViewportHandle viewportHandle_;

        /**
         * @brief Offsets from anchor point (top, right, bottom, left).
         */
        helios::math::vec4f offsets_{};

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

        public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        /**
         * @brief Default constructor.
         */
        UiTransformComponent() = default;

        /**
         * @brief Copy constructor.
         */
        UiTransformComponent(const UiTransformComponent&) = default;

        /**
         * @brief Copy assignment operator.
         */
        UiTransformComponent& operator=(const UiTransformComponent&) = default;

        /**
         * @brief Move constructor.
         */
        UiTransformComponent(UiTransformComponent&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         */
        UiTransformComponent& operator=(UiTransformComponent&&) noexcept = default;


        /**
         * @brief Sets the anchor point.
         *
         * @param anchor The anchor point relative to the viewport.
         */
        void setAnchor(const helios::ui::layout::Anchor anchor) noexcept {
            anchor_ = anchor;
        }

        /**
         * @brief Returns the current anchor point.
         *
         * @return The anchor point relative to the viewport.
         */
        [[nodiscard]] helios::ui::layout::Anchor anchor() const noexcept {
            return anchor_;
        }

        /**
         * @brief Sets the viewport ID.
         *
         * @param viewportHandle The handle of the viewport for positioning.
         */
        void setViewportHandle(const ViewportHandle viewportHandle) noexcept {
            viewportHandle_ = viewportHandle;
        }

        /**
         * @brief Sets the offsets from the anchor point.
         *
         * @param offsets Offsets as vec4f (top, right, bottom, left).
         */
        void setOffsets(const helios::math::vec4f offsets) noexcept {
            offsets_ = offsets;
        }

        /**
         * @brief Returns the current offsets.
         *
         * @return Offsets as vec4f (top, right, bottom, left).
         */
        [[nodiscard]] helios::math::vec4f offsets() const noexcept {
            return offsets_;
        }

        /**
         * @brief Sets the pivot point.
         *
         * @param pivot The pivot point of this element.
         */
        void setPivot(const helios::ui::layout::Anchor pivot) noexcept {
            pivot_ = pivot;
        }

        /**
         * @brief Returns the current pivot point.
         *
         * @return The pivot point of this element.
         */
        [[nodiscard]]  helios::ui::layout::Anchor pivot() const noexcept {
            return pivot_;
        }

        /**
         * @brief Returns the viewport handle.
         *
         * @return The handle of the viewport this element is positioned relative to.
         */
        [[nodiscard]] ViewportHandle viewportHandle() const noexcept {
            return viewportHandle_;
        }



    };

}