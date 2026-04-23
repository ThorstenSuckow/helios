module;

#include <cassert>

export module helios.core.components.Mat4Component;

import helios.math.types;

import helios.math.concepts;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::core::components {

    /**
     * @brief Generic 4x4 matrix component with dirty-state tracking.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for matrix values.
     */
    template<typename TDomainTag, typename THandle, typename TNumericType>
    requires IsNumeric<TNumericType>
    class Mat4Component  {

        helios::math::mat4<TNumericType> value_{};

        bool isDirty_ = true;

    public:

        Mat4Component() = default;

        /**
         * @brief Constructs the component with an initial value.
         *
         * @param value Initial value vector.
         */
        explicit Mat4Component(const mat4<TNumericType>& value) : value_(value){}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         *
         * @param other The component to copy from.
         */
        Mat4Component(const Mat4Component& other) :
            value_(other.value_),
            isDirty_(true) {}

        /** @brief Default copy assignment. */
        Mat4Component& operator=(const Mat4Component&) = default;
        /** @brief Default move constructor. */
        Mat4Component(Mat4Component&&) noexcept = default;
        /** @brief Default move assignment. */
        Mat4Component& operator=(Mat4Component&&) noexcept = default;

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
         * @return Current 4x4 matrix value.
         */
        [[nodiscard]] mat4<TNumericType>& value() noexcept {
            return value_;
        }

        [[nodiscard]] const mat4<TNumericType>& value() const noexcept {
            return value_;
        }

        /**
         * @brief Updates the value and marks the component dirty on change.
         *
         * @param value New value vector.
         */
        void setValue(const mat4<TNumericType> value) noexcept {

            if (value_.same(value)) {
                return;
            }

            value_ = value;
            isDirty_ = true;
        };


    };

}
