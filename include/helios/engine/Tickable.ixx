/**
* @file Tickable.ixx
 * @brief Abstract interface for objects that are updated (ticked) every frame.
 */

module;

export module helios.engine.Tickable;


export namespace helios::engine {

    /**
     * @class Tickable
     * @brief Abstract interface for objects that are updated (ticked) every frame.
     *
     * Implementors receive regular tick callbacks from the engine with the elapsed
     * time since the last update. Implementations should keep ticks fast and avoid
     * long blocking operations.
     */
    class Tickable {
    public:
        virtual ~Tickable() = default;

        /**
         * @brief Advance the object's internal state by deltaTime seconds.
         *
         * @param deltaTime Time elapsed since the last tick, in seconds.
         * @return A reference to *this to allow for chaining if desired.
         */
        virtual Tickable& tick(float deltaTime) = 0;

    };

}
