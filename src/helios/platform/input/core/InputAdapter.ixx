module;


import helios.platform.input.types;
import helios.platform.window.core;

export module helios.platform.input.core:InputAdapter;

using namespace helios::platform::window::core;

export namespace helios::platform::input::core {


    class InputAdapter {
        public:
            InputAdapter() = default;
            virtual ~InputAdapter() = default;

            [[nodiscard]] virtual bool isKeyPressed(Key key, const Window& win) const = 0;

    };

}
