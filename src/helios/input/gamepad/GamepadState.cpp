/**
 * @file GamepadState.cpp
 * @brief Implementation of the GamepadState class.
 */
module;

#include <algorithm>
#include <helios/helios_config.h>

module helios.input.gamepad.GamepadState;

import helios.math.types;


namespace helios::input::gamepad {

    GamepadState::GamepadState(
        const float axisLeftX, const float axisLeftY, const float axisRightX,
        const float axisRightY, const float triggerLeft, const float triggerRight,

        const bool buttonA, const bool buttonB, const bool buttonX, const bool buttonY,
        const bool buttonStart, const bool buttonBack, const bool buttonGuide,
        const bool buttonLeftBumper, const bool buttonRightBumper, const bool buttonLeftThumb,
        const bool buttonRightThumb, const bool buttonDpadUp, const bool buttonDpadRight,
        const bool buttonDpadDown, const bool buttonDpadLeft

    ) noexcept {
        updateAxes(
            axisLeftX, axisLeftY, axisRightX, axisRightY, triggerLeft, triggerRight,

           buttonA, buttonB, buttonX, buttonY, buttonStart, buttonBack, buttonGuide, buttonLeftBumper,
           buttonRightBumper, buttonLeftThumb, buttonRightThumb, buttonDpadUp, buttonDpadRight,
           buttonDpadDown, buttonDpadLeft);
    }


    void GamepadState::updateAxes(
        const float axisLeftX, const float axisLeftY, const float axisRightX, const float axisRightY,
        const float triggerLeft, const float triggerRight,

        const bool buttonA, const bool buttonB, const bool buttonX, const bool buttonY,
        const bool buttonStart, const bool buttonBack, const bool buttonGuide,
        const bool buttonLeftBumper, const bool buttonRightBumper, const bool buttonLeftThumb,
        const bool buttonRightThumb, const bool buttonDpadUp, const bool buttonDpadRight,
        const bool buttonDpadDown, const bool buttonDpadLeft
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

        buttonA_ = buttonA;
        buttonB_ = buttonB;
        buttonX_ = buttonX;
        buttonY_ = buttonY;
        buttonStart_ = buttonStart;
        buttonBack_ = buttonBack;
        buttonGuide_ = buttonGuide;
        buttonLeftBumper_ = buttonLeftBumper;
        buttonRightBumper_ = buttonRightBumper;
        buttonLeftThumb_ = buttonLeftThumb;
        buttonRightThumb_ = buttonRightThumb;
        buttonDpadUp_ = buttonDpadUp;
        buttonDpadRight_ = buttonDpadRight;
        buttonDpadDown_ = buttonDpadDown;
        buttonDpadLeft_ = buttonDpadLeft;

        needsUpdate_ = true;
    }


    void GamepadState::update() const noexcept {
        left_ = helios::math::vec2f(axisLeftX_, axisLeftY_);
        right_ = helios::math::vec2f(axisRightX_, axisRightY_);
        trigger_ = helios::math::vec2f(triggerLeft_, triggerRight_);

        needsUpdate_ = false;
    }

}

