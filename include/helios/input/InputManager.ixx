module;

#include <memory>
#include <stdexcept>

export module helios.input.InputManager;

import helios.input.InputAdapter;
import helios.input.types.Key;
import helios.window.Window;
import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::input::InputManager"
export namespace helios::input {

    /**
     * InputManager providing access to various input states.
     *
     * The `InputManager` acts as an intermediary between the application and
     * the underlying systems which require some sort of input mechanism.
     * The raw events are processed by InputAdapters this InputManagers
     * owns.
     */
    class InputManager  {

        private:
        /**
         * A pointer to the currently observed window, which might be nullptr.
         */
        const helios::window::Window* observedWin_ = nullptr;

        /**
         * The InputAdapter owned by this InputManager.
         */
        std::unique_ptr<const helios::input::InputAdapter> input_;

        protected:
        /**
         * The const reference to the logger used with this Adapter.
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);


        public:
        /**
         * Creates a new InputManager with the specified InputAdapter.
         * Ownership of the InputAdapter is transferred to this InputManager.
         *
         * @param input The InputAdapter used with this InputManager.
         */
        explicit InputManager(std::unique_ptr<const helios::input::InputAdapter> input);


        /**
         * Sets the window this InputManager will observe for input.
         * All subsequent input queries will be performed
         * on this window.
         *
         * @param win a const reference to the window to observe
         */
        void observe(const helios::window::Window& win) noexcept;


        /**
         * Returns a non-owning pointer to the currently observed window.
         * Returns a nullptr if no window is currently observed.
         *
         * @return The currently observed window as a non owning cont pointer,
         * or nullptr.
         */
        const helios::window::Window* observedWindow() const;


        /**
         * Polls events from the currently observed window.
         * Calls the `pollEvents()` method of the observed window to process
         * any pending window-related input events.
         * This method should be called regularly, preferably once per frame.
         *
         * @see Window::pollEvents
         *
         * @param deltaTime The time elapsed since the last frame, in seconds.
         */
        void poll(float deltaTime) const noexcept;


        /**
         * Returns true if the specified key is currently pressed, otherwise false.
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


    };



}