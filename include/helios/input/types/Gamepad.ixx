module;

export module helios.input.types.Gamepad;

export namespace helios::input::types {

    /**
     * @brief Enumerates common keyboard keys.
     *
     * This enumeration provides identifiers for gamepads, commonly identified
     * by a name representing the "port" they are mapped to. Values are
     * in convenient bitmask order.
     *
     * It's intended to be a platform-agnostic representation of physical gamepads.
     *
     * Implementing APIs are advised to properly map their implementation against
     * this enumeration.
     */
    enum Gamepad {
        ONE   = 1, // 0001
        TWO   = 2, // 0010
        THREE = 4, // 0100
        FOUR  = 8, // 1000



        /**
         * @brief Provides the total count of the items in this enumeration.
         */
        SIZE = 4
    };

}