module;

#include <memory>
#include <stdexcept>

export module helios.input.InputManager;

import helios.input.types.Gamepad;
import helios.input.GamepadState;
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
     * the underlying systems which require some sort of input mechanism.
     * The raw events are processed by InputAdapters this InputManagers
     * owns.
     *
     * An InputManager allows for querying GamepadState-objects by
     * calling gamepadState(). The method allows for passing the id of the gamepad for which
     * the input state should be returned.
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
         * @brief A Bitmask used for registering the Gamepads that should be polled for inputs.
         */
        unsigned int gamepadMask_ = 0x00;

        protected:
        /**
         * @brief The const reference to the logger used with this Adapter.
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);


        public:
        /**
         * @brief Creates a new InputManager with the specified InputAdapter.
         *
         * Ownership of the InputAdapter is transferred to this InputManager.
         *
         * @param input The InputAdapter used with this InputManager.
         */
        explicit InputManager(std::unique_ptr<helios::input::InputAdapter> input);


        /**
         * @brief Sets the window this InputManager will observe for input.
         *
         * All subsequent input queries will be performed
         * on this window.
         *
         * @param win a const reference to the window to observe
         */
        void observe(const helios::window::Window& win) noexcept;


        /**
         * @brief Returns a non-owning pointer to the currently observed window.
         *
         * Returns a nullptr if no window is currently observed.
         *
         * @return The currently observed window as a non owning cont pointer,
         * or nullptr.
         */
        const helios::window::Window* observedWindow() const;


        /**
         * @brief Polls events from the currently observed window and registered Gamepads.
         *
         * Calls the `pollEvents()` method of the observed window to process
         * any pending window-related input events.
         * This method should be called regularly, preferably once per frame.
         * For updating GamepadState with their current input states, make sure to
         * call registerGamepads() with a bitmask representing the gamepads to poll.
         *
         * @see Window::pollEvents
         *
         * @param deltaTime The time elapsed since the last frame, in seconds.
         */
        void poll(float deltaTime)  noexcept;


        /**
         * @brief Returns true if the specified key is currently pressed, otherwise false.
         *
         * This method delegates to the underlying InputAdapter.
         *
         * @see InputAdapter::isKeyPressed
         *
         * @param key the key to check for the pressed-state
         *
         * @return true if the key is pressed, otherwise false. This method also returns false
         * if the observed window is not set (nullptr).
         */
        [[nodiscard]] bool isKeyPressed(const helios::input::types::Key& key) const noexcept;


        /**
         * @brief Explicitly tells this InputManager which gamepads to poll for input states
         * in poll().
         *
         * @param gamepadMask A bitmask representing the gamepad ids that should be observed by
         * this InputManager, i.e. registerGamepads(Gamepad::ONE | Gamepad::THREE).
         *
         * @return The bitmask this InputManager uses for polling gamepad states. Defaults
         * to a bitmask that represents no Gamepads at all if an ínvalid mask was submitted .
         */
        unsigned int registerGamepads(unsigned int gamepadMask) noexcept;


        /**
         * @brief Returns a const ref of the GamepadState for the specified gamepadId.
         *
         * This method queries the InputAdapter owned by this InputManager for the
         * GamepadState identified by the specified gamepadId.
         * If no GamepadState was found under the specified gamepadId, this method
         * returns a GamepadState object initialized to its default values.
         * To test whether a gamepad is available, use isConnected().
         * For lifetime management of the GamepadState, see the documentation in InputAdapter.
         *
         * GamepadState objects get updated with poll().
         *
         * @param gamepadId The id of the gamepad for which the GamepadState is queried.
         *
         * @return The const ref GamepadState for the specified gamepad.
         *
         * @see isConnected()
         * @see poll()
         */
        [[nodiscard]] const GamepadState& gamepadState(helios::input::types::Gamepad gamepadId) const noexcept;

        /**
         * Return true if the specified gamepad is connected, otherwise false.
         *
         * @param gamepadId The id of the gamepad to test.
         *
         * @return true if a gamepad was found for the specified gamepadId, otherwise false.
         */
        [[nodiscard]] bool isConnected(helios::input::types::Gamepad gamepadId) const noexcept;

    };



}