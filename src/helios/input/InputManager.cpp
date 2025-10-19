module;

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
        std::unique_ptr<const helios::input::InputAdapter> input)
            : input_(std::move(input)) {}

    void InputManager::observe(const helios::window::Window& win) noexcept {
        observedWin_ = &win;
    }


    const helios::window::Window* InputManager::observedWindow() const {
        return observedWin_;
    }


    void InputManager::poll(float deltaTime) const noexcept {
        if (observedWin_ != nullptr) {
            observedWin_->pollEvents();
        } else {
            logger_.warn("No window to observe.");
        }
    }


    [[nodiscard]] bool InputManager::isKeyPressed(const helios::input::types::Key& key) const noexcept {
        if (observedWin_ == nullptr) {
            logger_.warn("No window to observe.");
            return false;
        }

        return input_->isKeyPressed(key, *observedWin_);
    }


};
