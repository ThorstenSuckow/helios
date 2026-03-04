/**
 * @file System.ixx
 * @brief Abstract base class for game systems operating on the GameWorld.
 */
module;


export module helios.engine.runtime.world.System;


import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::runtime::world {

    /**
     * @brief Abstract base class for game systems.
     *
     * @details Systems are global game logic processors that operate on the
     * GameWorld rather than individual GameObjects — ideal for cross-cutting
     * concerns like physics, collision detection, or object pooling.
     *
     * Concrete systems are plain classes that provide `update(UpdateContext&)`
     * (required) and optionally `init(GameWorld&)`. They are wrapped
     * automatically by TypedSystem<T> when registered via SystemRegistry.
     *
     * @see TypedSystem
     * @see SystemRegistry
     */
    class System {

    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived systems.
         */
        virtual ~System() = default;

        /**
         * @brief Called when the system is initialized by the GameWorld.
         *
         * @param gameWorld The GameWorld this system is being added to.
         */
        virtual void initSystem(helios::engine::runtime::world::GameWorld& gameWorld) noexcept = 0;

        /**
         * @brief Called each frame to update the system.
         *
         * @param updateContext Context containing deltaTime, input state, and
         *                      references to CommandBuffer and GameWorld.
         */
        virtual void updateSystem(helios::engine::runtime::world::UpdateContext& updateContext) noexcept = 0;

    };


}