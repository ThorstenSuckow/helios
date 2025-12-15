/**
 * @file InputAdapter.ixx
 * @brief Abstract interface for platform-specific input adapters.
 */
module;

#include <bit>
#include <memory>
#include <array>
#include <utility>

export module helios.input.InputAdapter;

import helios.input.types.Gamepad;
import helios.input.gamepad.GamepadState;
import helios.input.types.Key;
import helios.window.Window;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.input.gamepad.GamepadSettings;
import helios.input.gamepad.DeadzoneStrategy;

#define HELIOS_LOG_SCOPE "helios::input::InputAdapter"
export namespace helios::input {

    /**
     * @brief Abstract interface for platform-specific input adapters.
     *
     * Provides a unified interface for querying input device states across different
     * platforms. Concrete implementations translate generic input queries into
     * platform-specific API calls.
     *
     * The adapter manages per-gamepad configuration through `GamepadSettings` and applies
     * input normalization via a configurable `DeadzoneStrategy`.
     *
     * @see GLFWInputAdapter for a GLFW-based implementation.
     * @see GamepadSettings for per-controller configuration options.
     * @see DeadzoneStrategy for input normalization strategies.
     */
    class InputAdapter {

        protected:
        /**
         * @brief Shared logger instance for all InputAdapter objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Strategy used to normalize analog stick input within deadzones.
         *
         * Ownership is held by the adapter. Applied during gamepad state updates
         * to filter out hardware drift and rescale input values.
         */
        std::unique_ptr<helios::input::gamepad::DeadzoneStrategy> deadzoneStrategy_;

        /**
         * @brief Per-gamepad configuration settings.
         *
         * Array indexed by gamepad ID, storing deadzone thresholds and axis
         * inversion flags for each connected controller.
         */
        std::array<helios::input::gamepad::GamepadSettings, std::to_underlying(helios::input::types::Gamepad::size_)> gamepadSettings_ = {};


        public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~InputAdapter() = default;

        /**
         * @brief Constructs an InputAdapter with the specified deadzone strategy.
         *
         * @param deadzoneStrategy The strategy used for analog stick normalization.
         *                         Ownership is transferred to this adapter.
         */
        explicit InputAdapter(std::unique_ptr<helios::input::gamepad::DeadzoneStrategy> deadzoneStrategy) :
        deadzoneStrategy_(std::move(deadzoneStrategy))
        {}

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
        [[nodiscard]] virtual const helios::input::gamepad::GamepadState& gamepadState(
            helios::input::types::Gamepad gamepadId) const noexcept = 0;


        /**
         * @brief Returns the configuration settings for the specified gamepad.
         *
         * Provides access to the mutable settings object for the given gamepad,
         * allowing runtime configuration of deadzone thresholds and axis inversion.
         * Changes to the returned settings are applied during subsequent calls
         * to `updateGamepadState()`.
         *
         * @param gamepadId The gamepad to retrieve settings for.
         *
         * @return Reference to the mutable GamepadSettings for the specified gamepad.
         *
         * @see GamepadSettings for available configuration options.
         */
        [[nodiscard]] helios::input::gamepad::GamepadSettings& gamepadSettings(helios::input::types::Gamepad gamepadId) noexcept {
            const auto id = static_cast<unsigned int>(gamepadId);
            const auto idx = std::countr_zero(id);

            return gamepadSettings_[idx];
        }
    };

}
