/**
 * @file InputManager.ixx
 * @brief InputManager providing access to various input states.
 */
module;

#include <memory>
#include <stdexcept>
#include <cassert>
#include <cmath>

export module helios.input.InputManager;

import helios.input.types.Gamepad;
import helios.input.gamepad.GamepadState;
import helios.input.InputAdapter;
import helios.input.types.Key;
import helios.window.Window;
import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::input::InputManager"
export namespace helios::input {

    /**
     * @brief InputManager providing access to various input states.
     *
     * The `InputManager` acts as an intermediary between the application and
     * the underlying systems that provide input. Raw events are processed by
     * InputAdapters owned by this `InputManager`.
     *
     * An `InputManager` allows querying `GamepadState` objects by calling
     * `gamepadState()`. The method accepts an id that identifies the
     * gamepad whose input state should be returned.
     */
    class InputManager  {

        private:
        /**
         * @brief A pointer to the currently observed window, which might be nullptr.
         */
        const helios::window::Window* observedWin_ = nullptr;

        /**
         * @brief The InputAdapter owned by this InputManager.
         */
        std::unique_ptr<helios::input::InputAdapter> input_;

        /**
         * @brief A bitmask used for registering the gamepads that should be polled for inputs.
         */
        unsigned int gamepadMask_ = 0x00;

        protected:
        /**
         * @brief Shared logger instance for all InputManager objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        public:
        /**
         * @brief Creates a new InputManager with the specified InputAdapter.
         *
         * Ownership of the InputAdapter is transferred to this InputManager.
         *
         * @param input The InputAdapter used with this InputManager.
         */
        explicit InputManager(std::unique_ptr<helios::input::InputAdapter> input)
            : input_(std::move(input)) {}


        /**
         * @brief Sets the window this InputManager will observe for input.
         *
         * All subsequent input queries will be performed on this window.
         *
         * @param win A const reference to the window to observe.
         */
        void observe(const helios::window::Window& win) noexcept {
            observedWin_ = &win;
        }


        /**
         * @brief Returns a non-owning pointer to the currently observed window.
         *
         * Returns `nullptr` if no window is currently observed.
         *
         * @return A non-owning pointer to the currently observed window, or `nullptr`.
         */
        [[nodiscard]] const helios::window::Window* observedWindow() const {
            return observedWin_;
        }


        /**
         * @brief Polls events from the currently observed window and registered gamepads.
         *
         * Calls the `pollEvents()` method of the observed window to process
         * any pending window-related input events. This method should be called
         * regularly, preferably once per frame. For updating `GamepadState`
         * objects with their current input states, call `registerGamepads()`
         * with a bitmask representing the gamepads to poll.
         *
         * @see Window::pollEvents
         *
         * @param deltaTime The time elapsed since the last frame, in seconds.
         */
        void poll(float deltaTime)  noexcept {
            if (observedWin_ != nullptr) {
                observedWin_->pollEvents();
            } else {
                logger_.warn("No window to observe.");
            }
            input_->updateGamepadState(gamepadMask_);
        }


        /**
         * @brief Returns true if the specified key is currently pressed, otherwise false.
         *
         * This method delegates to the underlying `InputAdapter`.
         *
         * @see InputAdapter::isKeyPressed
         *
         * @param key The key to check for the pressed state.
         *
         * @return True if the key is pressed; returns false if the observed window is not set.
         */
        [[nodiscard]] bool isKeyPressed(const helios::input::types::Key& key) const noexcept {
            if (observedWin_ == nullptr) {
                logger_.warn("No window to observe.");
                return false;
            }

            return input_->isKeyPressed(key, *observedWin_);
        }

        /**
         * @brief Returns true if the specified key is currently released, otherwise false.
         *
         * This method delegates to the underlying `InputAdapter`.
         *
         * @see InputAdapter::isKeyReleased
         *
         * @param key The key to check for the released state.
         *
         * @return True if the key is released; returns false if the observed window is not set.
         */
        [[nodiscard]] bool isKeyReleased(const helios::input::types::Key& key) const noexcept {
            if (observedWin_ == nullptr) {
                logger_.warn("No window to observe.");
                return false;
            }

            return input_->isKeyReleased(key, *observedWin_);
        }


        /**
         * @brief Explicitly tells this InputManager which gamepads to poll for input states
         * in `poll()`.
         *
         * @param gamepadMask A bitmask representing the gamepad ids that should be observed by
         * this InputManager (e.g. `registerGamepads(Gamepad::ONE | Gamepad::THREE)`).
         *
         * @return The bitmask this InputManager uses for polling gamepad states. If an invalid
         * mask is provided, the method returns a mask that represents no gamepads.
         */
        unsigned int registerGamepads(unsigned int mask) noexcept {
            const unsigned int maxMask = static_cast<int>(std::pow(
                helios::input::types::Gamepad::size_,
                2.0f
            ) - 1);
            assert(mask <= maxMask && "mask out of bounds");
            if (mask > maxMask) {
                logger_.warn("Gamepad mask out of bounds. Clamping to maximum valid value.");
                mask = maxMask;
            }


            gamepadMask_ = mask;

            return gamepadMask_;
        }


        /**
         * @brief Returns a const reference to the `GamepadState` for the specified `gamepadId`.
         *
         * This method queries the `InputAdapter` owned by this `InputManager` for the
         * `GamepadState` identified by the specified `gamepadId`. If no `GamepadState` exists
         * for the given id, this method returns a `GamepadState` object initialized to its
         * default values. To test whether a gamepad is available, use `isConnected()`.
         *
         * `GamepadState` objects are updated by calling `poll()`.
         *
         * @param gamepadId The id of the gamepad for which the `GamepadState` is queried.
         *
         * @return A const reference to the `GamepadState` for the specified gamepad.
         *
         * @see isConnected()
         * @see poll()
         */
        [[nodiscard]] const helios::input::gamepad::GamepadState& gamepadState(helios::input::types::Gamepad gamepadId) const noexcept {
            return input_->gamepadState(gamepadId);
        }

        /**
         * @brief Returns true if the specified gamepad is connected, otherwise false.
         *
         * @param gamepadId The id of the gamepad to test.
         *
         * @return True if a gamepad was found for the specified `gamepadId`, otherwise false.
         */
        [[nodiscard]] bool isConnected(helios::input::types::Gamepad gamepadId) const noexcept {
            return input_->isConnected(gamepadId);
        }


        /**
         * @brief Retrieves the InputAdapter associated with this InputManager.
         *
         * @return A reference to the InputAdapter associated with this InputManager.
         */
        [[nodiscard]] const helios::input::InputAdapter& inputAdapter() const noexcept {
            return *input_;
        }

        /**
         * @brief Retrieves the InputAdapter associated with this InputManager.
         *
         * This non-const overload allows modification of InputAdapter settings,
         * such as gamepad deadzone and axis inversion configuration.
         *
         * @return A reference to the InputAdapter associated with this InputManager.
         */
        [[nodiscard]] helios::input::InputAdapter& inputAdapter() noexcept {
            return *input_;
        }

    };



} // namespace helios::input