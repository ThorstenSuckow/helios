/**
 * @file System.ixx
 * @brief Base class for game systems that operate on the GameWorld.
 */
module;


export module helios.engine.game.System;

import helios.engine.game.Updatable;

export namespace helios::engine::game {

    class GameWorld;

    /**
     * @brief Abstract base class for game systems.
     *
     * @details Systems are global game logic processors that operate on the GameWorld
     * rather than individual GameObjects, ideal for cross-cutting concerns
     * like physics simulation, collision detection, or object pooling.
     *
     * Systems are added to a GameWorld and receive update calls each frame.
     * The `onAdd()` callback provides access to the GameWorld for initialization.
     *
     * Example usage:
     * ```cpp
     * class PhysicsSystem : public System {
     * public:
     *     void update(UpdateContext& ctx) noexcept override {
     *         // Apply physics to all rigid bodies
     *     }
     * };
     *
     * gameWorld.addSystem(std::make_unique<PhysicsSystem>());
     * ```
     */
    class System : public helios::engine::game::Updatable {

    protected:

        /**
         * @brief Pointer to the GameWorld this system belongs to.
         *
         * @details Set by `onAdd()` when the system is registered with a GameWorld.
         * Non-owning pointer; valid for the lifetime of the system.
         */
        helios::engine::game::GameWorld* gameWorld_ = nullptr;

    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived systems.
         */
        virtual ~System() = default;

        /**
         * @brief Called when the system is added to a GameWorld.
         *
         * @details Override this method to perform initialization that requires
         * access to the GameWorld (e.g., querying the scene graph, registering
         * event handlers).
         *
         * @param gameWorld The GameWorld this system is being added to.
         */
        virtual void onAdd(helios::engine::game::GameWorld* gameWorld) noexcept {
            gameWorld_ = gameWorld;

        }

        /**
         * @brief Updates the system each frame.
         *
         * @param updateContext The update context containing frame timing information.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override = 0;

    };


}