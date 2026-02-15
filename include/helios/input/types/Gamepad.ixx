/**
 * @file Gamepad.ixx
 * @brief Identifiers for supported gamepads.
 */
module;

export module helios.input.types.Gamepad;

export namespace helios::input::types {

    /**
     * @brief Bitmask enum for gamepad input buttons.
     *
     * @details Each value represents a single button as a bit flag,
     * allowing efficient combination and querying of multiple inputs.
     */
    enum class GamepadInput {

        /**
         * @brief Start button.
         */
        Start = 1 << 0,

        /**
         * @brief Select/Back button.
         */
        Select = 1 << 5,

        /**
         * @brief A button (bottom face button).
         */
        A = 1 << 1,

        /**
         * @brief B button (right face button).
         */
        B = 1 << 2,

        /**
         * @brief X button (left face button).
         */
        X = 1 << 3,

        /**
         * @brief Y button (top face button).
         */
        Y = 1 << 4,

        /**
         * @brief Left stick button (L3).
         */
        LeftStick = 1 << 6,

        /**
         * @brief Right stick button (R3).
         */
        RightStick = 1 << 7,

        /**
         * @brief D-pad up.
         */
        Up = 1 << 8,

        /**
         * @brief D-pad down.
         */
        Down = 1 << 9,

        /**
         * @brief D-pad left.
         */
        Left = 1 << 10,

        /**
         * @brief D-pad right.
         */
        Right = 1 << 11,

        /**
         * @brief Left bumper (L1).
         */
        LeftBumper = 1 << 12,

        /**
         * @brief Right bumper (R1).
         */
        RightBumper = 1 << 13,

        /**
         * @brief Left trigger (L2).
         */
        LeftTrigger = 1 << 14,

        /**
         * @brief Right trigger (R2).
         */
        RightTrigger = 1 << 15
    };

    /**
     * @brief Enumerates gamepad identifiers in bitmask-friendly values.
     *
     * This enumeration provides identifiers for gamepads, commonly identified
     * by the logical port they are mapped to. Values are chosen for convenient
     * bitmask composition (ONE | TWO, ...).
     *
     * Implementing APIs should map their platform-specific gamepad indices to
     * these identifiers.
     */
    enum Gamepad {
        ONE   = 1, // 0001
        TWO   = 2, // 0010
        THREE = 4, // 0100
        FOUR  = 8, // 1000



        /**
         * @brief Number of logical gamepad entries represented by this enum.
         */
        size_ = 4
    };

}