module;

export module helios.engine.core:Tickable;


export namespace helios::engine::core {

    class Tickable {
    public:
        virtual ~Tickable() = default;
        virtual Tickable& tick(float deltaTime) = 0;

    };

}

