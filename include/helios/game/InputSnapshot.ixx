/**
 * @file InputSnapshot.ixx
 * @brief Immutable snapshot of input state at a specific point in time.
 */
module;


export module helios.game.InputSnapshot;

import helios.input.GamepadState;

export namespace helios::game {

    /**
     * @brief Immutable snapshot capturing input state at a specific point in time.
     *
     * @details InputSnapshot decouples input polling from input processing by capturing
     * the complete input state at a single moment. This immutable design enables deterministic
     * replay where snapshots can be recorded and replayed exactly, network synchronization by
     * transmitting snapshots to remote clients, frame-consistent processing where all systems
     * see identical input state, and multi-threaded safety as snapshots can be shared across
     * threads without locking.
     *
     * The current implementation captures only gamepad input. Future versions may extend
     * this to include keyboard, mouse, and other input devices.
     *
     * Typical usage:
     * ```cpp
     * // Capture input at start of frame
     * auto snapshot = helios::game::InputSnapshot(
     *     inputManager.gamepadState(helios::input::types::Gamepad::ONE)
     * );
     *
     * // Process input (potentially in parallel or deferred)
     * inputHandler.handleInput(snapshot, player, commandBuffer, deltaTime);
     * ```
     *
     * @note The snapshot is immutable after construction and reflects input state at
     *       construction time. Currently only supports a single gamepad; multi-input
     *       support is planned.
     *
     * @todo Extend to support multiple gamepads, keyboard, and mouse input.
     * @todo Add timestamp to enable precise replay and network synchronization.
     */
    class InputSnapshot {

        /**
         * @brief Captured gamepad state.
         *
         * @details Stored as a const member to enforce immutability of the snapshot.
         */
        const helios::input::GamepadState gamepadState_;

    public:

        /**
         * @brief Constructs an InputSnapshot capturing the current gamepad state.
         *
         * @param gamepadState The gamepad state to capture. Copied into the snapshot.
         *
         * @note This operation is noexcept and cheap as GamepadState is a small struct.
         *       The snapshot remains valid even if the source state is modified or destroyed.
         */
        explicit InputSnapshot(const helios::input::GamepadState& gamepadState) noexcept
        : gamepadState_(gamepadState) {
        }

        /**
         * @brief Returns the captured gamepad state.
         *
         * @return Const reference to the captured GamepadState, valid for the lifetime
         *         of this InputSnapshot. The state is immutable and reflects input at
         *         the time of snapshot creation.
         */
        [[nodiscard]] const helios::input::GamepadState& gamepadState() const noexcept {
            return gamepadState_;
        }
    };

}