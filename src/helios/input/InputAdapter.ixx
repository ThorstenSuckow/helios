module;

export module helios.input.InputAdapter;

import helios.input.types.Key;
import helios.window.Window;
import helios.util.log.Logger;
import helios.util.log.LogManager;


#define HELIOS_LOG_SCOPE "helios::input::InputAdapter"
export namespace helios::input {

    /**
     * Abstract interface for querying various input states.
     *
     * The `InputAdapter` defines a contract for classes that provide
     * platform-specific querying.
     * Implementations are responsible for integrating native systems
     * to determine the states of keyboards, game controllers etc.
     *
     */
    class InputAdapter {

        protected:
            /**
             * The const reference to the logger used with this Adapter.
             */
            const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().logger(HELIOS_LOG_SCOPE);

        public:

        InputAdapter() = default;

        virtual ~InputAdapter() = default;

        /**
         * Returns true if the key is pressed, otherwise false.
         *
         * @param key The key to query for the `pressed` state.
         * @param win The window instance from which the state should be queried.
         *
         * @return True if the key is pressed, otherwise false.
         */
        [[nodiscard]] virtual bool isKeyPressed(
            helios::input::types::Key key,
            const helios::window::Window& win) const noexcept = 0;

    };

}
