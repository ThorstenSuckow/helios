/**
 * @file System.ixx
 * @brief Type-erased system wrapper using the Concept/Model pattern.
 */
module;

#include <cassert>
#include <memory>

export module helios.runtime.world.System;


import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;

import helios.engine.common.concepts;

using namespace helios::runtime::world;
using namespace helios::engine::common::concepts;

export namespace helios::runtime::world {


    /**
     * @brief Type-erased wrapper for game logic processors.
     *
     * @details System uses the Concept/Model pattern to erase the concrete
     * system type. Concrete systems are plain classes that satisfy
     * `HasUpdate<T>` — they do not inherit from System.
     *
     * The internal `Concept` base defines the virtual interface, and
     * `Model<T>` adapts the concrete type T, owning it by value.
     * `init()` is conditionally forwarded if `HasInit<T>` is satisfied.
     *
     * System is move-only (non-copyable).
     *
     * @see HasUpdate
     * @see HasInit
     * @see SystemRegistry
     *
     * @see https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure
     */
    class System {

    private:
        /**
         * @brief Internal virtual interface for type erasure.
         */
        class Concept {
        public:

            virtual ~Concept() = default;
            virtual void update(UpdateContext& updateContext) noexcept = 0;
            virtual void init(GameWorld& gameWorld) noexcept = 0;
            virtual void* underlying() noexcept = 0;
            virtual const void* underlying() const noexcept = 0;
        };

        /**
         * @brief Typed wrapper that adapts a concrete system to the Concept interface.
         *
         * @tparam T The concrete system type, must satisfy `HasUpdate<T>`.
         */
        template<typename T>
        class Model final : public Concept {
            T system_;

        public:


            explicit Model(T sys) :  system_(std::move(sys)) {}

            void update(UpdateContext& updateContext) noexcept override {
                system_.update(updateContext);
            }
            void init(GameWorld& gameWorld) noexcept override {
                if constexpr (HasInit<T>) {
                    system_.init(gameWorld);
                }
            }

            void* underlying() noexcept override {
                return &system_;
            }

            const void* underlying() const noexcept override {
                return &system_;
            }
        };

        std::unique_ptr<Concept> pimpl_;

    public:

        /**
         * @brief Default constructor creating an empty System.
         */
        System() = default;

        /**
         * @brief Wraps a concrete system in a type-erased System.
         *
         * @tparam T The concrete system type, must satisfy `HasUpdate<T>`.
         *
         * @param system The concrete system instance to wrap (moved into internal storage).
         */
        template<typename T>
        requires HasUpdate<T>
        explicit System(T system) : pimpl_(std::make_unique<Model<T>>(std::move(system))) {}

        System(const System&) = delete;
        System& operator=(const System&) = delete;

        System& operator=(System&&) = default;
        System(System&&) noexcept = default;


        /**
         * @brief Delegates to the wrapped system's `update()` method.
         *
         * @param updateContext The current frame's update context.
         *
         * @pre System must be initialized (pimpl_ != nullptr).
         */
        void update(UpdateContext& updateContext) noexcept {
            assert(pimpl_ && "System not initialized");
            pimpl_->update(updateContext);
        }

        /**
         * @brief Delegates to the wrapped system's `init()` method, if present.
         *
         * @details If the concrete type satisfies `HasInit<T>`, its `init()` is
         * called. Otherwise this is a no-op.
         *
         * @param gameWorld The GameWorld for one-time initialization.
         *
         * @pre System must be initialized (pimpl_ != nullptr).
         */
        void init(GameWorld& gameWorld) noexcept {
            assert(pimpl_ && "System not initialized");
            pimpl_->init(gameWorld);
        }

        /**
         * @brief Returns a type-erased pointer to the wrapped system instance.
         *
         * @return Pointer to the underlying concrete system.
         *
         * @pre System must be initialized (pimpl_ != nullptr).
         */
        [[nodiscard]] void* underlying() noexcept {
            assert(pimpl_ && "System not initialized");
            return pimpl_->underlying();
        }

        /**
         * @copydoc underlying()
         */
        [[nodiscard]] const void* underlying() const noexcept {
            assert(pimpl_ && "System not initialized");
            return pimpl_->underlying();
        }

    };


}