/**
 * @file Updatable.ixx
 * @brief Interface for components that require per-frame updates.
 */
module;


export module helios.engine.ecs.Updatable;

import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::ecs {

    struct UpdateContext;

    /**
     * @brief Interface for components that require per-frame updates.
     *
     * @details Components implementing this interface will have their update()
     * method called each frame by the owning GameObject. This enables frame-dependent
     * behaviors such as physics simulation, animation, and input processing.
     *
     * Example implementation:
     * ```cpp
     * class PhysicsComponent : public Component, public Updatable {
     * public:
     *     void update(UpdateContext& ctx) noexcept override {
     *         velocity_ += gravity_ * ctx.deltaTime;
     *         position_ += velocity_ * ctx.deltaTime;
     *     }
     * };
     * ```
     *
     * @note Implementations must be noexcept to prevent exceptions from propagating
     *       during the game loop.
     */
    class Updatable {

    public:
        virtual ~Updatable() = default;
        Updatable() = default;

        /**
         * @brief Called each frame to update the component state.
         *
         * @param updateContext Context containing deltaTime, input state, and references
         *                      to CommandBuffer and GameWorld.
         */
        virtual void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept = 0;
    };

}