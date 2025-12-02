module;


export module helios.game.InputSnapshot;

import helios.input.GamepadState;

export namespace helios::game {

    /**
     * Collects available input at a specific point in time.
     * The current implementation only takes snapshots of a single Gamepad.
     */
    class InputSnapshot {

        const helios::input::GamepadState gamepadState_;
    public:

        explicit InputSnapshot(const helios::input::GamepadState& gamepadState) noexcept
        : gamepadState_(gamepadState) {
        }

        const helios::input::GamepadState& gamepadState() const {
            return gamepadState_;
        }
    };

}