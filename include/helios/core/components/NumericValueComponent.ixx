/**
 * @file NumericValueComponent.ixx
 * @brief Generic scalar component with dirty-state tracking.
 */
module;

#include <cassert>
#include <algorithm>
#include <cmath>

export module helios.core.components.NumericValueComponent;

import helios.math;

using namespace helios::math;
using namespace helios::math::concepts;
export namespace helios::core::components {

    /**
     * @brief Generic numeric value component with dirty-state tracking.
     *
     * @tparam TDomainTag Semantic domain tag.
     * @tparam THandle Owning entity handle type.
     * @tparam TNumericType Scalar type for stored numeric values.
     */
    template<typename TDomainTag, typename THandle, typename TNumericType>
    requires IsNumeric<TNumericType>
    class NumericValueComponent  {

        TNumericType value_{};

        bool isDirty_ = true;

    public:

        NumericValueComponent() = default;


        /**
         * @brief Constructs the component with an initial scalar value.
         *
         * @param value Initial numeric value.
         */
        explicit NumericValueComponent(const TNumericType value) : value_(value){}

        /**
         * @brief Copy constructor.
         *
         * @details Copies the value and forces the copied component into a
         * dirty state to ensure dependent systems refresh cached data.
         *
         * @param other The component to copy from.
         */
        NumericValueComponent(const NumericValueComponent& other) :
            value_(other.value_),
            isDirty_(true) {}

        /** @brief Default copy assignment. */
        NumericValueComponent& operator=(const NumericValueComponent&) = default;
        /** @brief Default move constructor. */
        NumericValueComponent(NumericValueComponent&&) noexcept = default;
        /** @brief Default move assignment. */
        NumericValueComponent& operator=(NumericValueComponent&&) noexcept = default;

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
         * @return Current value.
         */
        [[nodiscard]] TNumericType value() noexcept {
            return value_;
        }

        [[nodiscard]] TNumericType value() const noexcept {
            return value_;
        }

        /**
         * @brief Updates the value and marks the component dirty on change.
         *
         * @details Integral types use exact comparison. Floating-point types use
         * `helios::math::EPSILON_LENGTH` to suppress insignificant updates.
         *
         * @param value New scalar value.
         */
        void setValue(const TNumericType value) noexcept {

            if constexpr (std::floating_point<TNumericType>) {
                if (std::abs(value_ - value) <= static_cast<TNumericType>(helios::math::EPSILON_LENGTH)) {
                    return;
                }
            } else {
                if (value_ == value) {
                    return;
                }
            }


            value_ = value;
            isDirty_ = true;
        };


    };

}
