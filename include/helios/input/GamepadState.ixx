/**
 * @brief A lightweight class for transferring the state of a Gamepad.
 */
module;

#include <cassert>
#include <algorithm>

export module helios.input.GamepadState;

import helios.math.types;


export namespace helios::input {

    /**
     * @brief A lightweight class for transferring the state of a Gamepad.
     *
     * This class models a reusable object representing the input state of a gamepad.
     * The input state of a gamepad consists of the state of the left and right trigger,
     * ranging from [0, 1], where 1 means fully pressed and 0 means not pressed at all.
     *
     * Similarly, a GamepadState provides information about the left and the right
     * gamepad axes. Both the x- and the y-axis range from [-1, 1]. For the x-axis,
     * the following holds:
     *  -  0.00 => not moved at all
     *  - -1.00 => moved all the way to the left
     *  -  1.00 => moved all the way to the right
     *
     * For the y-axis the positive direction corresponds to "up" and the negative
     * direction corresponds to "down".
     *
     * Updating the values is done by calling `updateAxes` on this object.
     *
     * For convenient access of the axes in 2D coordinates, the class provides accessors
     * to the sticks' axes in `vec2f` form.
     *
     * @note Implementations must account for joystick drift: achieving exactly 0.0
     * for x/y when no human input occurred is rare. Consider applying a dead zone
     * when processing the GamepadState. Axis values are individually clipped to [-1.0, 1.0],
     * but the magnitude of the resulting (x, y) vector may exceed 1.0. Applications should
     * normalize input vectors where appropriate and possibly offer calibration.
     *
     * This implementation is inspired by the GLFW gamepad input model.
     * @see https://www.glfw.org/docs/latest/input_guide.html#joystick
     */
    class GamepadState {

    private:
        /**
         * @brief Flag to indicate if the cached vec2f members need to be recalculated.
         */
        mutable bool needsUpdate_ = true;

        /**
         * @brief Raw float value of the left stick's x-axis.
         */
        float axisLeftX_{};

        /**
         * @brief Raw float value of the left stick's y-axis.
         */
        float axisLeftY_{};

        /**
         * @brief Raw float value of the right stick's x-axis.
         */
        float axisRightX_{};

        /**
         * @brief Raw float value of the right stick's y-axis.
         */
        float axisRightY_{};

        /**
         * @brief Raw float value of the left trigger.
         */
        float triggerLeft_{};

        /**
         * @brief Raw float value of the right trigger.
         */
        float triggerRight_{};

        /**
         * @brief Cached vec2f representation of the left stick (x, y).
         */
        mutable helios::math::vec2f left_;

        /**
         * @brief Cached vec2f representation of the right stick (x, y).
         */
        mutable helios::math::vec2f right_;

        /**
         * @brief Cached vec2f representation of the triggers (left, right).
         */
        mutable helios::math::vec2f trigger_;

        /**
         * @brief Internal method to update the cached vec2f representations.
         * This method should be called when the current values of the axes are queried
         * in vec2f form and `needsUpdate_` evaluates to `true`.
         */
        void update() const noexcept {
            left_ = helios::math::vec2f(axisLeftX_, axisLeftY_);
            right_ = helios::math::vec2f(axisRightX_, axisRightY_);
            trigger_ = helios::math::vec2f(triggerLeft_, triggerRight_);

            needsUpdate_ = false;
        }

    public:

        ~GamepadState() = default;

        /**
         * @brief Default constructor.
         *
         * Initializes all the values with 0.0f.
         */
        GamepadState() = default;

        /**
         * @brief Creates a new GamepadState object.
         *
         * Delegates to `updateAxes` for value initialization.
         *
         * @see updateAxes
         */
        explicit GamepadState(
            const float axisLeftX, const float axisLeftY,
            const float axisRightX, const float axisRightY,
            const float triggerLeft, const float triggerRight
        ) noexcept {
            updateAxes(
                axisLeftX, axisLeftY,
                axisRightX, axisRightY,
                triggerLeft, triggerRight
            );
        }


        /**
         * @brief Updates the axes of this GamepadState object.
         *
         * For the sticks' range [-1, 1] the following holds:
         * -1 means moved all the way left/down, 0 means not moved at all, 1 means moved all the way right/up.
         *
         * For the triggers' range of [0, 1] the following holds:
         * 0 means not pressed, 1 means fully pressed.
         *
         * @param axisLeftX The value of the x-axis of the left stick, in the range [-1, 1].
         * @param axisLeftY The value of the y-axis of the left stick, in the range [-1, 1].
         * @param axisRightX The value of the x-axis of the right stick, in the range [-1, 1].
         * @param axisRightY The value of the y-axis of the right stick, in the range [-1, 1].
         * @param triggerLeft The value of the left trigger, in the range [0, 1].
         * @param triggerRight The value of the right trigger, in the range [0, 1].
         */
        void updateAxes (
            const float axisLeftX, const float axisLeftY,
            const float axisRightX, const float axisRightY,
            const float triggerLeft, const float triggerRight
        ) noexcept{

            assert(axisLeftX >= -1.0f && axisLeftX <= 1.0f && "axisLeftX is out of bounds.");
            assert(axisLeftY >= -1.0f && axisLeftY <= 1.0f && "axisLeftY is out of bounds.");
            assert(axisRightX >= -1.0f && axisRightX <= 1.0f && "axisRightX is out of bounds.");
            assert(axisRightY >= -1.0f && axisRightY <= 1.0f && "axisRightY is out of bounds.");
            assert(triggerLeft >= 0.0f && triggerLeft <= 1.0f && "triggerLeft is out of bounds.");
            assert(triggerRight >= 0.0f && triggerRight <= 1.0f && "triggerRight is out of bounds.");

            axisLeftX_    = std::clamp(axisLeftX, -1.0f, 1.0f);
            axisLeftY_    = std::clamp(axisLeftY, -1.0f, 1.0f);
            axisRightX_   = std::clamp(axisRightX, -1.0f, 1.0f);
            axisRightY_   = std::clamp(axisRightY, -1.0f, 1.0f);
            triggerLeft_  = std::clamp(triggerLeft, 0.0f, 1.0f);
            triggerRight_ = std::clamp(triggerRight, 0.0f, 1.0f);

            needsUpdate_ = true;
        }

        /**
         * @brief Returns the current value of the left stick's x-axis.
         *
         * @return float value in the range [-1, 1]
         */
        [[nodiscard]] float leftX() const noexcept {
            return axisLeftX_;
        }


        /**
         * @brief Returns the current value of the left stick's y-axis.
         *
         * @return float value in the range [-1, 1]
         */
        [[nodiscard]] float leftY() const noexcept {
            return axisLeftY_;
        }


        /**
         * @brief Returns the current value of the right stick's x-axis.
         *
         * @return float value in the range [-1, 1]
         */
        [[nodiscard]] float rightX() const noexcept {
            return axisRightX_;
        }


        /**
         * @brief Returns the current value of the right stick's y-axis.
         *
         * @return float value in the range [-1, 1]
         */
        [[nodiscard]] float rightY() const noexcept {
            return axisRightY_;
        }


        /**
         * @brief Returns the current value of the left trigger.
         *
         * @return float value in the range [0, 1]
         */
        [[nodiscard]] float triggerLeft() const noexcept {
            return triggerLeft_;
        }


        /**
         * @brief Returns the current value of the right trigger.
         *
         * @return float value in the range [0, 1]
         */
        [[nodiscard]] float triggerRight() const noexcept {
            return triggerRight_;
        }


        /**
         * @brief Returns the state of the left stick as a helios::math::vec2f.
         *
         * @return A helios::math::vec2f, with the first component being the x-axis, the second component the y-axis.
         */
        [[nodiscard]] helios::math::vec2f left()  const noexcept {
            if (needsUpdate_) {
                update();
            }
            return left_;
        }


        /**
         * @brief Returns the state of the right stick as a helios::math::vec2f.
         *
         * @return A helios::math::vec2f, with the first component being the x-axis, the second component the y-axis.
         */
        [[nodiscard]] helios::math::vec2f right() const noexcept {
            if (needsUpdate_) {
                update();
            }
            return right_;
        }


        /**
         * @brief Returns the state of the triggers as a helios::math::vec2f.
         *
         * @return A helios::math::vec2f, with the first component being the left trigger-axis,
         * the second component the right trigger-axis.
         */
        [[nodiscard]] helios::math::vec2f trigger() const noexcept {
            if (needsUpdate_) {
                update();
            }
            return trigger_;
        }
    };


}