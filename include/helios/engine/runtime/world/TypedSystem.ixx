/**
 * @file TypedSystem.ixx
 * @brief Type-erasing wrapper that adapts a plain system type to the System interface.
 */
module;

#include <concepts>

export module helios.engine.runtime.world.TypedSystem;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.System;


export namespace helios::engine::runtime::world {

    /**
     * @brief Concept detecting an optional `init(GameWorld&)` method.
     *
     * @tparam T The system type to check.
     */
    template<typename T>
    concept HasInit = requires(T& t, helios::engine::runtime::world::GameWorld& gameWorld) {
        {t.init(gameWorld) } -> std::same_as<void>;
    };

    /**
     * @brief Concept requiring a `update(UpdateContext&)` method.
     *
     * @tparam T The system type to check.
     */
    template<typename T>
    concept HasUpdate = requires(T& t, helios::engine::runtime::world::UpdateContext& updateContext) {
        {t.update(updateContext) } -> std::same_as<void>;
    };


    /**
     * @brief Type-erasing wrapper that adapts a plain system type to the System interface.
     *
     * @details TypedSystem owns an instance of T by value and delegates the
     * abstract `initSystem()` / `updateSystem()` calls from System to
     * `T::init()` and `T::update()` respectively.
     *
     * - `T::update(UpdateContext&)` is **required** (enforced via `static_assert`).
     * - `T::init(GameWorld&)` is **optional** (detected via the HasInit concept).
     *
     * The SystemRegistry creates `TypedSystem<T>` internally; user code
     * never interacts with this wrapper directly.
     *
     * @tparam T The concrete system type. Must satisfy HasUpdate.
     *
     * @see System
     * @see SystemRegistry
     * @see HasInit
     * @see HasUpdate
     */
    template<typename T>
    class TypedSystem : public System {

        /**
         * @brief The owned system instance.
         */
        T system_;

    public:

        /**
         * @brief Constructs a TypedSystem by moving an existing system instance.
         *
         * @param system The system instance to wrap.
         */
        explicit TypedSystem(T system) : system_(std::move(system)) {}

        /**
         * @brief Constructs a TypedSystem by forwarding arguments to the T constructor.
         *
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the T constructor.
         */
        template<typename... Args>
        explicit TypedSystem(Args&&... args) : system_(std::forward<Args>(args)...) {}

        /**
         * @brief Returns a mutable reference to the underlying system.
         *
         * @return Reference to the wrapped T instance.
         */
        T& underlying() noexcept {
            return system_;
        }

        /**
         * @brief Returns a const reference to the underlying system.
         *
         * @return Const reference to the wrapped T instance.
         */
        const T& underlying() const noexcept {
            return system_;
        }

        /**
         * @brief Initializes the system with a reference to the GameWorld.
         *
         * @details Conditionally calls `T::init(GameWorld&)` if the wrapped
         * type satisfies HasInit.
         *
         * @param gameWorld The GameWorld this system belongs to.
         */
        void initSystem(helios::engine::runtime::world::GameWorld& gameWorld) noexcept final {
            if constexpr (HasInit<T>) {
                system_.init(gameWorld);
            }
        }

        /**
         * @brief Delegates the per-frame update to the wrapped system.
         *
         * @param updateContext Context for the current frame.
         */
        void updateSystem(helios::engine::runtime::world::UpdateContext& updateContext) noexcept final {
            static_assert(HasUpdate<T>);
            system_.update(updateContext);
        }

    };


}