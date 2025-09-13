module;


import helios.platform.input.types;
import helios.platform.core;

export module helios.platform.input.core:InputAdapter;

export namespace helios::platform::input::core {


    class InputAdapter {
        public:
            InputAdapter() = default;
            virtual ~InputAdapter() = default;

            [[nodiscard]] virtual bool isKeyPressed(Key key, const Window& win) const = 0;

    };

}
