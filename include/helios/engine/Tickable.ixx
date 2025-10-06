module;

export module helios.engine.Tickable;


export namespace helios::engine {

    class Tickable {
    public:
        virtual ~Tickable() = default;
        virtual Tickable& tick(float deltaTime) = 0;

    };

}

