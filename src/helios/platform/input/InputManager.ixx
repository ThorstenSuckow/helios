module;

#include <memory>
#include <stdexcept>

export module helios.platform.input:InputManager;

import helios.platform.input.core;
import helios.platform.input.types;
import helios.platform.window.core;
import helios.engine.core;

using namespace helios::platform::window::core;

export namespace helios::platform::input {

    class InputManager : public engine::core::Tickable {

        private:
            Window* observedWin_ = nullptr;
            std::unique_ptr<const core::InputAdapter> input_;

        public:

        explicit InputManager(std::unique_ptr<const core::InputAdapter> input):
        input_(std::move(input)) {}

        InputManager& observe(Window& win) noexcept {
            observedWin_ = &win;
            return *this;
        }

        const Window* observedWindow() const {
            return observedWin_;
        }

        Tickable& tick(float deltaTime) override {
            if (observedWin_ != nullptr) {
                observedWin_->pollEvents();
            }

            return *this;
        }


        [[nodiscard]] bool isKeyPressed(const Key& key) const {
            if (observedWin_ == nullptr) {
                throw std::runtime_error("Expected observedWin_ to be available, but none found.");
            }
            return input_->isKeyPressed(key, *observedWin_);
        }


    };



}