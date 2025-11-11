/**
 * @file Gamepad.ixx
 * @brief Identifiers for supported gamepads.
 */
module;

export module helios.input.types.Gamepad;

export namespace helios::input::types {

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