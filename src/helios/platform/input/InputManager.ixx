module;

#include <memory>

export module helios.platform.input:InputManager;

import helios.platform.input.core;
import helios.platform.input.types;
import helios.platform.core;

export namespace helios::platform::input {

    class InputManager {

        private:
            std::unique_ptr<const core::InputAdapter> input_;

        public:

        explicit InputManager(std::unique_ptr<const core::InputAdapter> input):
        input_(std::move(input)) {}

        [[nodiscard]] bool isKeyPressed(const Key& key, const Window& win) const noexcept {
            return input_->isKeyPressed(key, win);
        }


    };



}