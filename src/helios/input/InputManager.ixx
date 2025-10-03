module;

#include <memory>
#include <stdexcept>

export module helios.input.InputManager;

import helios.input.InputAdapter;
import helios.input.types.Key;
import helios.window.Window;

export namespace helios::input {

    class InputManager  {

        private:
            helios::window::Window* observedWin_ = nullptr;
            std::unique_ptr<const helios::input::InputAdapter> input_;

        public:

        explicit InputManager(std::unique_ptr<const helios::input::InputAdapter> input):
        input_(std::move(input)) {}

        InputManager& observe(helios::window::Window& win) noexcept {
            observedWin_ = &win;
            return *this;
        }

        const helios::window::Window* observedWindow() const {
            return observedWin_;
        }

        InputManager& poll(float deltaTime) noexcept {
            if (observedWin_ != nullptr) {
                observedWin_->pollEvents();
            }

            return *this;
        }


        [[nodiscard]] bool isKeyPressed(const helios::input::types::Key& key) const {
            if (observedWin_ == nullptr) {
                throw std::runtime_error("Expected observedWin_ to be available, but none found.");
            }
            return input_->isKeyPressed(key, *observedWin_);
        }


    };



}