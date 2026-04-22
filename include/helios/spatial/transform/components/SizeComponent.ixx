module;

#include <cassert>

export module helios.spatial.transform.components.SizeComponent;

import helios.runtime.world.GameObject;
import helios.math.types;

import helios.core.spatial.Transform;
import helios.core.units.Unit;
import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::spatial::transform::components {

    /**
     * @brief Component storing 2D size and dirty-state tracking.
     *
     * @details `SizeComponent` is used for entities that expose width/height-like
     * extents in a domain-specific coordinate system. Any mutation marks the
     * component as dirty so update systems can recalculate dependent state lazily.
     *
     * @tparam TDomainTag Domain marker type (for strong typing across spaces).
     * @tparam THandle Entity handle type this component is attached to.
     * @tparam TNumericType Scalar type of the 2D size vector.
     */
    template<typename TDomainTag, typename THandle, typename TNumericType>
    requires IsNumeric<TNumericType>
    class SizeComponent  {

        helios::math::vec2<TNumericType> size_{};

        bool isDirty_ = true;

    public:

        /**
         * @brief Constructs the component with an initial size.
         *
         * @param size Initial size vector.
         */
        explicit SizeComponent(const vec2<TNumericType> size) : size_(size){}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the size value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         *
         * @param other The component to copy from.
         */
        SizeComponent(const SizeComponent& other) :
            size_(other.size_),
            isDirty_(true) {}

        /** @brief Default copy assignment. */
        SizeComponent& operator=(const SizeComponent&) = default;
        /** @brief Default move constructor. */
        SizeComponent(SizeComponent&&) noexcept = default;
        /** @brief Default move assignment. */
        SizeComponent& operator=(SizeComponent&&) noexcept = default;

        /**
         * @brief Lifecycle hook called when the component is acquired.
         *
         * @details Marks the component dirty to trigger downstream recomputation.
         */
        void onAcquire() noexcept {
            isDirty_ = true;
        }

        /**
         * @brief Lifecycle hook called when the component is released.
         *
         * @details Marks the component dirty to trigger downstream recomputation.
         */
        void onRelease() noexcept {
            isDirty_ = true;
        }

        /**
         * @brief Clears the dirty flag after dependent systems consumed updates.
         */
        void clearDirty() noexcept {
            isDirty_ = false;
        }

        /**
         * @brief Returns whether the component requires a refresh pass.
         *
         * @return `true` if size changed or lifecycle hooks marked dirty.
         */
        [[nodiscard]] bool isDirty() const noexcept {
            return isDirty_;
        }

        /**
         * @brief Returns the current size.
         *
         * @return Current 2D size vector.
         */
        [[nodiscard]] vec2<TNumericType> size() const noexcept {
            return size_;
        }

        /**
         * @brief Updates the size and marks the component dirty on change.
         *
         * @param size New size vector.
         */
        void setSize(const vec2<TNumericType> size) noexcept {

            if (size_.same(size)) {
                return;
            }

            size_ = size;
            isDirty_ = true;
        };


    };

}
