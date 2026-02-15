/**
 * @file GamepadState.ixx
 * @brief Representation of current gamepad input state.
 */
module;

#include <algorithm>
#include <cassert>
#include <helios/helios_config.h>

export module helios.input.gamepad.GamepadState;

import helios.math.types;
import helios.util.log.Logger;
import helios.util.log.LogManager;

import helios.input.types.Gamepad;


#define HELIOS_LOG_SCOPE "helios::input::gamepad::GamepadState"
export namespace helios::input::gamepad {

    using namespace helios::input::types;

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
     * Button states are generally represented with boolean values, i.e. true for pressed,
     * otherwise false.
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

        uint32_t currInput_ = 0;
        uint32_t prevInput_ = 0;

        /**
         * @brief Shared logger instance for all GamepadState objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        /**
         * @brief State of the A button (true if pressed).
         */
        bool buttonA_ = false;
        bool prevButtonA_ = false;

        /**
         * @brief State of the B button (true if pressed).
         */
        bool buttonB_ = false;
        bool prevButtonB_ = false;

        /**
         * @brief State of the X button (true if pressed).
         */
        bool buttonX_ = false;

        /**
         * @brief State of the Y button (true if pressed).
         */
        bool buttonY_ = false;

        /**
         * @brief State of the Start button (true if pressed).
         */
        bool buttonStart_ = false;

        bool prevButtonStart_ = false;

        /**
         * @brief State of the Back button (true if pressed).
         */
        bool buttonBack_ = false;

        /**
         * @brief State of the Guide button (true if pressed).
         */
        bool buttonGuide_ = false;

        /**
         * @brief State of the left bumper button (true if pressed).
         */
        bool buttonLeftBumper_ = false;

        /**
         * @brief State of the right bumper button (true if pressed).
         */
        bool buttonRightBumper_ = false;

        /**
         * @brief State of the left thumbstick button (true if pressed).
         */
        bool buttonLeftThumb_ = false;

        /**
         * @brief State of the right thumbstick button (true if pressed).
         */
        bool buttonRightThumb_ = false;

        /**
         * @brief State of the D-pad up button (true if pressed).
         */
        bool buttonDpadUp_ = false;
        bool prevButtonDpadUp_ = false;

        /**
         * @brief State of the D-pad right button (true if pressed).
         */
        bool buttonDpadRight_ = false;

        /**
         * @brief State of the D-pad down button (true if pressed).
         */
        bool buttonDpadDown_ = false;
        bool prevButtonDpadDown_ = false;

        /**
         * @brief State of the D-pad left button (true if pressed).
         */
        bool buttonDpadLeft_ = false;

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
            float axisLeftX, float axisLeftY, float axisRightX,
            float axisRightY, float triggerLeft, float triggerRight,

            bool buttonA, bool buttonB, bool buttonX, bool buttonY,
            bool buttonStart, bool buttonBack, bool buttonGuide,
            bool buttonLeftBumper, bool buttonRightBumper, bool buttonLeftThumb,
            bool buttonRightThumb, bool buttonDpadUp, bool buttonDpadRight,
            bool buttonDpadDown, bool buttonDpadLeft

        ) noexcept {
            updateAxes(
                axisLeftX, axisLeftY, axisRightX, axisRightY, triggerLeft, triggerRight,

               buttonA, buttonB, buttonX, buttonY, buttonStart, buttonBack, buttonGuide, buttonLeftBumper,
               buttonRightBumper, buttonLeftThumb, buttonRightThumb, buttonDpadUp, buttonDpadRight,
               buttonDpadDown, buttonDpadLeft);
        }


        /**
         * @brief Updates the axes and button states of this GamepadState object.
         *
         * This method updates raw axis and trigger values and stores the boolean
         * state of all standard gamepad buttons. Float parameters are asserted to
         * be in their expected ranges and then clamped.
         *
         * For the sticks' range [-1, 1] the following holds:
         * -1 means moved all the way left/down, 0 means not moved at all, 1 means moved all the way right/up.
         *
         * For the triggers' range of [0, 1] the following holds:
         * 0 means not pressed, 1 means fully pressed.
         *
         * @param axisLeftX The x-axis of the left stick, expected in [-1, 1].
         * @param axisLeftY The y-axis of the left stick, expected in [-1, 1].
         * @param axisRightX The x-axis of the right stick, expected in [-1, 1].
         * @param axisRightY The y-axis of the right stick, expected in [-1, 1].
         * @param triggerLeft The left trigger value, expected in [0, 1].
         * @param triggerRight The right trigger value, expected in [0, 1].
         * @param buttonA True if the A button is pressed.
         * @param buttonB True if the B button is pressed.
         * @param buttonX True if the X button is pressed.
         * @param buttonY True if the Y button is pressed.
         * @param buttonStart True if the Start button is pressed.
         * @param buttonBack True if the Back button is pressed.
         * @param buttonGuide True if the Guide (platform) button is pressed.
         * @param buttonLeftBumper True if the left bumper is pressed.
         * @param buttonRightBumper True if the right bumper is pressed.
         * @param buttonLeftThumb True if the left thumbstick button is pressed.
         * @param buttonRightThumb True if the right thumbstick button is pressed.
         * @param buttonDpadUp True if the D-pad Up button is pressed.
         * @param buttonDpadRight True if the D-pad Right button is pressed.
         * @param buttonDpadDown True if the D-pad Down button is pressed.
         * @param buttonDpadLeft True if the D-pad Left button is pressed.
         */
        void updateAxes(
            float axisLeftX, float axisLeftY, float axisRightX, float axisRightY,
            float triggerLeft, float triggerRight,

            bool buttonA, bool buttonB, bool buttonX, bool buttonY,
            bool buttonStart, bool buttonBack, bool buttonGuide,
            bool buttonLeftBumper, bool buttonRightBumper, bool buttonLeftThumb,
            bool buttonRightThumb, bool buttonDpadUp, bool buttonDpadRight,
            bool buttonDpadDown, bool buttonDpadLeft
        ) noexcept {

#ifdef HELIOS_DEBUG
            if(axisLeftX < -1.0f || axisLeftX > 1.0f) {
                logger_.warn("axisLeftX is out of bounds.");
            }
            if(axisLeftY < -1.0f || axisLeftY > 1.0f) {
                logger_.warn("axisLeftY is out of bounds.");
            }
            if(axisRightX < -1.0f || axisRightX > 1.0f) {
                logger_.warn("axisRightX is out of bounds.");
            }
            if(axisRightY < -1.0f || axisRightY > 1.0f) {
                logger_.warn("axisRightY is out of bounds.");
            }
            if(triggerLeft < 0.0f || triggerLeft > 1.0f) {
                logger_.warn("triggerLeft is out of bounds.");
            }
            if(triggerRight < 0.0f || triggerRight > 1.0f) {
                logger_.warn("triggerRight is out of bounds.");
            }
#endif

            axisLeftX_ = std::clamp(axisLeftX, -1.0f, 1.0f);
            axisLeftY_ = std::clamp(axisLeftY, -1.0f, 1.0f);
            axisRightX_ = std::clamp(axisRightX, -1.0f, 1.0f);
            axisRightY_ = std::clamp(axisRightY, -1.0f, 1.0f);
            triggerLeft_ = std::clamp(triggerLeft, 0.0f, 1.0f);
            triggerRight_ = std::clamp(triggerRight, 0.0f, 1.0f);
;

            prevButtonA_ = buttonA_;
            buttonA_ = buttonA;

            prevButtonB_ = buttonB_;
            buttonB_ = buttonB;

            buttonX_ = buttonX;

            buttonY_ = buttonY;

            prevButtonStart_ = buttonStart_;
            buttonStart_ = buttonStart;

            buttonBack_ = buttonBack;

            buttonGuide_ = buttonGuide;

            buttonLeftBumper_ = buttonLeftBumper;

            buttonRightBumper_ = buttonRightBumper;

            buttonLeftThumb_ = buttonLeftThumb;

            buttonRightThumb_ = buttonRightThumb;

            prevButtonDpadUp_ = buttonDpadUp_;
            buttonDpadUp_ = buttonDpadUp;

            buttonDpadRight_ = buttonDpadRight;

            prevButtonDpadDown_ = buttonDpadDown_;
            buttonDpadDown_ = buttonDpadDown;

            buttonDpadLeft_ = buttonDpadLeft;

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
        [[nodiscard]] helios::math::vec2f left() const noexcept {
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


        /**
         * @brief Returns true when the A button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonA() const noexcept {
            return buttonA_;
        };


        /**
         * @brief Returns true when the B button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonB() const noexcept {
            return buttonB_;
        };


        /**
         * @brief Returns true when the X button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonX() const noexcept {
            return buttonX_;
        };


        /**
         * @brief Returns true when the Y button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonY() const noexcept {
            return buttonY_;
        };


        /**
         * @brief Checks if a button is currently held down.
         *
         * @param input The button to check.
         *
         * @return True if the button is currently pressed.
         */
        [[nodiscard]] bool isButtonDown(const GamepadInput input) const {
            switch (input) {
                case GamepadInput::Start:
                    return buttonStart_;
                case GamepadInput::Down:
                    return buttonDpadDown_;
                case GamepadInput::Up:
                    return buttonDpadUp_;
                case GamepadInput::A:
                    return buttonA_;
                case GamepadInput::B:
                    return buttonB_;

                default:
                    assert(false && "Unexpected input");
                    return false;
            }
        }

        /**
         * @brief Checks if a button has been held for at least 2 frames.
         *
         * @param input The button to check.
         *
         * @return True if the button was pressed last frame and is still pressed.
         */
        [[nodiscard]] bool isButtonHeld(const GamepadInput input) const {
            switch (input) {
                case GamepadInput::Start:
                    return prevButtonStart_ && buttonStart_;
                case GamepadInput::Down:
                    return prevButtonDpadDown_ && buttonDpadDown_;
                case GamepadInput::Up:
                    return prevButtonDpadUp_ && buttonDpadUp_;
                case GamepadInput::A:
                    return prevButtonA_ && buttonA_;
                case GamepadInput::B:
                    return prevButtonB_ && buttonB_;

                default:
                    assert(false && "Unexpected input");
                    return false;
            }
        }

        /**
         * @brief Checks if a button was released this frame.
         *
         * @param input The button to check.
         *
         * @return True if the button was pressed last frame but is now released.
         */
        [[nodiscard]] bool isButtonUp(const GamepadInput input) const {
            switch (input) {
                case GamepadInput::Start:
                    return prevButtonStart_ && !buttonStart_;
                case GamepadInput::Down:
                    return prevButtonDpadDown_ && !buttonDpadDown_;
                case GamepadInput::Up:
                    return prevButtonDpadUp_ && !buttonDpadUp_;
                case GamepadInput::A:
                    return prevButtonA_ && !buttonA_;
                case GamepadInput::B:
                    return prevButtonB_ && !buttonB_;

                default:
                    assert(false && "Unexpected input");
                    return false;
            }
        }


        /**
         * @brief Checks if a button was pressed this frame.
         *
         * @param input The button to check.
         *
         * @return True if the button was not pressed last frame but is pressed now.
         */
        [[nodiscard]] bool isButtonPressed(const GamepadInput input) const {
            switch (input) {
                case GamepadInput::Start:
                    return !prevButtonStart_ && buttonStart_;
                case GamepadInput::Down:
                    return !prevButtonDpadDown_ && buttonDpadDown_;
                case GamepadInput::Up:
                    return !prevButtonDpadUp_ && buttonDpadUp_;
                case GamepadInput::A:
                    return !prevButtonA_ && buttonA_;
                case GamepadInput::B:
                    return !prevButtonB_ && buttonB_;

                default:
                    assert(false && "Unexpected input");
                    return false;
            }
        }



        /**
         * @brief Returns true when the Start button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonStart() const noexcept {
            return prevButtonStart_ && buttonStart_;
        };

        /**
         * @brief Returns true if Start was pressed this frame.
         *
         * @return True if Start was just pressed.
         */
        [[nodiscard]] bool buttonPressedStart() const noexcept {
            return !prevButtonStart_ && buttonStart_;
        };


        /**
         * @brief Returns true when the Back button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonBack() const noexcept {
            return buttonBack_;
        };


        /**
         * @brief Returns true when the Guide button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonGuide() const noexcept {
            return buttonGuide_;
        };


        /**
         * @brief Returns true when the left bumper is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonLeftBumper() const noexcept {
            return buttonLeftBumper_;
        };


        /**
         * @brief Returns true when the right bumper is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonRightBumper() const noexcept {
            return buttonRightBumper_;
        };


        /**
         * @brief Returns true when the left thumbstick button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonLeftThumb() const noexcept {
            return buttonLeftThumb_;
        };


        /**
         * @brief Returns true when the right thumbstick button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonRightThumb() const noexcept {
            return buttonRightThumb_;
        };


        /**
         * @brief Returns true when the D-pad Up button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonDpadUp() const noexcept {
            return buttonDpadUp_;
        };


        /**
         * @brief Returns true when the D-pad Right button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonDpadRight() const noexcept {
            return buttonDpadRight_;
        };


        /**
         * @brief Returns true when the D-pad Down button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonDpadDown() const noexcept {
            return buttonDpadDown_;
        };


        /**
         * @brief Returns true when the D-pad Left button is pressed.
         * @return true if pressed, false otherwise.
         */
        [[nodiscard]] bool buttonDpadLeft() const noexcept {
            return buttonDpadLeft_;
        };
    };


} // namespace helios::input
