/**
 * @file Vec4Component.ixx
 * @brief Generic 4D vector component with dirty-state tracking.
 */
module;

#include <cassert>

export module helios.core.components.Vec4Component;

import helios.math.types;

import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::core::components {

    /**
     * @brief Generic 4D value component with dirty-state tracking.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for vector values.
     */
    template<typename TDomainTag, typename THandle, typename TNumericType>
    requires IsNumeric<TNumericType>
    class Vec4Component  {

        helios::math::vec4<TNumericType> value_{};

        bool isDirty_ = true;

    public:

        Vec4Component() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial value vector.
         */
        explicit Vec4Component(const vec4<TNumericType> value) : value_(value){}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         *
         * @param other The component to copy from.
         */
        Vec4Component(const Vec4Component& other) :
            value_(other.value_),
            isDirty_(true) {}

        /** @brief Default copy assignment. */
        Vec4Component& operator=(const Vec4Component&) = default;
        /** @brief Default move constructor. */
        Vec4Component(Vec4Component&&) noexcept = default;
        /** @brief Default move assignment. */
        Vec4Component& operator=(Vec4Component&&) noexcept = default;

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
         * @return `true` if value changed or lifecycle hooks marked dirty.
         */
        [[nodiscard]] bool isDirty() const noexcept {
            return isDirty_;
        }

        /**
         * @brief Returns the current value.
         *
         * @return Current 4D vector value.
         */
        [[nodiscard]] vec4<TNumericType> value() const noexcept {
            return value_;
        }

        [[nodiscard]] vec4<TNumericType> value() noexcept {
            return value_;
        }

        /**
         * @brief Updates the value and marks the component dirty on change.
         *
         * @param value New value vector.
         */
        void setValue(const vec4<TNumericType> value) noexcept {

            if (value_.same(value)) {
                return;
            }

            value_ = value;
            isDirty_ = true;
        };


    };

}
