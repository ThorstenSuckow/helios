module;


import helios.input.types.Key;
import helios.window.Window;

export module helios.input.InputAdapter;

export namespace helios::input {


    class InputAdapter {
        public:
            InputAdapter() = default;
            virtual ~InputAdapter() = default;

            [[nodiscard]] virtual bool isKeyPressed(
                helios::input::types::Key key,
                const helios::window::Window& win) const = 0;

    };

}
