module;

export module helios.input.types.Key;

export namespace helios::input::types {

    /**
     * @brief Enumerates common keyboard keys.
     *
     * This enumeration provides identifiers for various keyboard keys.
     * It's intended to be a platform-agnostic representation of physical keys.
     *
     * Implementing APIs are advised to properly map their implementation against
     * this enumeration.
     */
    enum Key {
        ESC,


        /**
         * @brief Provides the total count of the items in this enumeration.
         */
        SIZE
    };

}