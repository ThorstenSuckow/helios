/**
 * @file InputAdapter.ixx
 * @brief Abstract interface for platform-specific input adapters.
 */
module;

export module helios.input.InputAdapter;

import helios.input.types.Gamepad;
import helios.input.GamepadState;
import helios.input.types.Key;
import helios.window.Window;
import helios.util.log.Logger;
import helios.util.log.LogManager;


#define HELIOS_LOG_SCOPE "helios::input::InputAdapter"
export namespace helios::input {

    /**
     * @brief Abstract interface for platform-specific input adapters.
     */
    class InputAdapter {

        protected:
        /**
         * @brief The const reference to the logger used with this Adapter.
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);


        public:
        InputAdapter() = default;

        virtual ~InputAdapter() = default;

        /**
         * @brief Returns true if the key is pressed, otherwise false.
         *
         * @param key The key to query for the `pressed` state.
         * @param win The window instance from which the state should be queried.
         *
         * @return True if the key is pressed, otherwise false.
         */
        [[nodiscard]] virtual bool isKeyPressed(
            helios::input::types::Key key,
            const helios::window::Window& win) const noexcept = 0;


        /**
         * @brief Returns a boolean value indicating the availability (i.e., connect state)
         * of the gamepad identified by the specified gamepadId.
         *
         * @param gamepadId The gamepadId to query for availability.
         *
         * @return true if the Gamepad identified by gamepadId is connected, otherwise false.
         */
        [[nodiscard]] virtual bool isConnected(helios::input::types::Gamepad gamepadId) const noexcept = 0;


        /**
         * @brief Updates the GamepadState objects with the values queried from the
         * underlying Gamepad identified by the specified mask.
         *
         * Updates the GamepadState objects with the current values of the underlying
         * hardware. If querying the underlying hardware fails, this method will
         * update the GamepadState values to a valid initial state (no movement or interaction).
         * Implementations should use `isConnected()` to check whether the current values of a
         * `GamepadState` object can be trusted.
         *
         * @param gamepadMask A bitmask representing all GamepadState objects to update,
         * e.g. Gamepad::ONE | Gamepad::TWO...
         */
        virtual void updateGamepadState(unsigned int gamepadMask) noexcept = 0;


        /**
         * @brief Returns a const ref to the GamepadState-object for the specified gamepadId.
         *
         * Implementing APIs can call this method in each frame to retrieve the current
         * state for the gamepad, as computed by updateGamepadState().
         * This method guarantees that for a given gamepadId, a single GamepadState object is
         * reused across calls, avoiding re-instantiation overhead.
         *
         * @param gamepadId The id of the gamepad to query
         *
         * @return Returns a const ref to the GamepadState. If the GamepadState was never queried
         * before, or if a recent call to updateGamepadState did not succeed, the values of the
         * GamepadState will not hold representative values. Implementing APIs should verify the
         * availability of the Gamepad by calling isConnected()
         *
         * @see updateGamepadState
         * @see isConnected
         */
        [[nodiscard]] virtual const GamepadState& gamepadState(
            helios::input::types::Gamepad gamepadId) const noexcept = 0;

    };

}
