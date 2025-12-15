module;

#include <cassert>
#include <memory>
#include <stdexcept>

module helios.input.InputManager;

import helios.input.InputAdapter;
import helios.input.types.Key;
import helios.window.Window;
import helios.util.log.Logger;
import helios.util.log.LogManager;

namespace helios::input {

    InputManager::InputManager(
        std::unique_ptr<helios::input::InputAdapter> input)
            : input_(std::move(input)) {}

    void InputManager::observe(const helios::window::Window& win) noexcept {
        observedWin_ = &win;
    }


    const helios::window::Window* InputManager::observedWindow() const {
        return observedWin_;
    }


    void InputManager::poll(float deltaTime) noexcept {
        if (observedWin_ != nullptr) {
            observedWin_->pollEvents();
        } else {
            logger_.warn("No window to observe.");
        }
        input_->updateGamepadState(gamepadMask_);
    }


    bool InputManager::isKeyPressed(const helios::input::types::Key& key) const noexcept {
        if (observedWin_ == nullptr) {
            logger_.warn("No window to observe.");
            return false;
        }

        return input_->isKeyPressed(key, *observedWin_);
    }

    const helios::input::gamepad::GamepadState& InputManager::gamepadState(const helios::input::types::Gamepad gamepadId) const noexcept {
        return input_->gamepadState(gamepadId);
    }

    bool InputManager::isConnected(helios::input::types::Gamepad gamepadId) const noexcept {
        return input_->isConnected(gamepadId);
    }

    unsigned int InputManager::registerGamepads(unsigned int mask) noexcept {
        const unsigned int maxMask = static_cast<int>(pow(helios::input::types::Gamepad::size_, 2) - 1);
        assert(mask <= maxMask && "mask out of bounds");
        if (mask > maxMask) {
            logger_.warn("Gamepad mask out of bounds. Clamping to maximum valid value.");
            mask = maxMask;
        }


        gamepadMask_ = mask;

        return gamepadMask_;

    }

};
