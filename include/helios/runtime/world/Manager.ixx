/**
 * @file Manager.ixx
 * @brief Type-erased manager wrapper using the Concept/Model pattern.
 */
module;

#include <cassert>
#include <memory>

export module helios.runtime.world.Manager;

import helios.engine.common.concepts.IsManagerLike;

import helios.runtime.world.UpdateContextFwd;
import helios.runtime.world.GameWorldFwd;


using namespace helios::engine::common::concepts;
export namespace helios::runtime::world {

    /**
     * @brief Concept detecting an optional `init(GameWorld&)` method on a manager.
     *
     * @tparam T The manager type to inspect.
     */
    template<typename T>
    concept HasInit = requires(T& t, GameWorld& gameWorld) {
        {t.init(gameWorld) } -> std::same_as<void>;
    };

    /**
     * @brief Concept detecting an optional `reset()` method on a manager.
     *
     * @tparam T The manager type to inspect.
     */
    template<typename T>
    concept HasReset = requires(T& t) {
            {t.reset() } -> std::same_as<void>;
    };



    /**
     * @brief Type-erased wrapper for game world managers.
     *
     * @details Manager uses the Concept/Model pattern to erase the concrete
     * manager type. Concrete managers are plain classes that satisfy
     * `IsManagerLike<T>` (i.e. provide `flush(UpdateContext&)` and declare
     * `using EngineRoleTag = ManagerRole;`).
     *
     * The internal `Concept` base defines the virtual interface, and
     * `Model<T>` adapts the concrete type T, owning it by value.
     * `init()` and `reset()` are conditionally forwarded if the concrete
     * type satisfies `HasInit<T>` or `HasReset<T>` respectively.
     *
     * Manager is move-only (non-copyable).
     *
     * @see IsManagerLike
     * @see ManagerRole
     * @see ManagerRegistry
     * @see ConceptModelRegistry
     */
    class Manager {

    private:
        /**
         * @brief Internal virtual interface for type erasure.
         */
        class Concept {
        public:
            virtual ~Concept() = default;
            virtual void flush(UpdateContext& updateContext) noexcept = 0;
            virtual void init(GameWorld& gameWorld) noexcept = 0;
            virtual void reset() noexcept = 0;

            [[nodiscard]] virtual void* underlying() noexcept = 0;
            [[nodiscard]] virtual const void* underlying() const noexcept = 0;
        };

        /**
         * @brief Typed wrapper that adapts a concrete manager to the Concept interface.
         *
         * @tparam T The concrete manager type, must satisfy `IsManagerLike<T>`.
         */
        template<typename T>
        class Model final : public Concept {
            T manager_;

            public:

            explicit Model(T sys) :  manager_(std::move(sys)) {}

            void flush(UpdateContext& updateContext) noexcept override {
                manager_.flush(updateContext);
            }
            void init(GameWorld& gameWorld) noexcept override {
                if constexpr (HasInit<T>) {
                    manager_.init(gameWorld);
                }
            }
            void reset() noexcept override {
                if constexpr (HasReset<T>) {
                    manager_.reset();
                }
            }

            void* underlying() noexcept override {
                return &manager_;
            }

            const void* underlying() const noexcept override {
                return &manager_;
            }
        };

        std::unique_ptr<Concept> pimpl_;

    public:

        /**
         * @brief Default constructor creating an empty Manager.
         */
        Manager() = default;

        /**
         * @brief Wraps a concrete manager in a type-erased Manager.
         *
         * @tparam T The concrete manager type, must satisfy `IsManagerLike<T>`.
         *
         * @param manager The concrete manager instance to wrap (moved into internal storage).
         */
        template<typename T>
        requires IsManagerLike<T>
        explicit Manager(T manager) : pimpl_(std::make_unique<Model<T>>(std::move(manager))) {}

        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

        Manager& operator=(Manager&&) = default;
        Manager(Manager&&) noexcept = default;


        /**
         * @brief Delegates to the wrapped manager's `flush()` method.
         *
         * @details Called by the GameLoop at each commit point after
         * CommandBuffers have been flushed. Managers process their
         * accumulated requests in batch.
         *
         * @param updateContext The current frame's update context.
         *
         * @pre Manager must be initialized (pimpl_ != nullptr).
         */
        void flush(UpdateContext& updateContext) noexcept {
            assert(pimpl_ && "Manager not initialized");
            pimpl_->flush(updateContext);
        }


        /**
         * @brief Delegates to the wrapped manager's `init()` method, if present.
         *
         * @details If the concrete type satisfies `HasInit<T>`, its `init()` is
         * called. Otherwise this is a no-op. Typically used by managers to
         * register their TypedCommandHandlers with the GameWorld.
         *
         * @param gameWorld The GameWorld for one-time initialization.
         *
         * @pre Manager must be initialized (pimpl_ != nullptr).
         */
        void init(GameWorld& gameWorld) noexcept {
            assert(pimpl_ && "Manager not initialized");
            pimpl_->init(gameWorld);
        }

        /**
         * @brief Delegates to the wrapped manager's `reset()` method, if present.
         *
         * @details If the concrete type satisfies `HasReset<T>`, its `reset()` is
         * called. Otherwise this is a no-op. Used during level transitions or
         * game restarts to clear accumulated state.
         *
         * @pre Manager must be initialized (pimpl_ != nullptr).
         */
        void reset() noexcept {
            assert(pimpl_ && "Manager not initialized");
            pimpl_->reset();
        }

        /**
         * @brief Returns a type-erased pointer to the wrapped manager instance.
         *
         * @return Pointer to the underlying concrete manager.
         *
         * @pre Manager must be initialized (pimpl_ != nullptr).
         */
        [[nodiscard]] void* underlying() noexcept {
            assert(pimpl_ && "Manager not initialized");
            return pimpl_->underlying();
        }

        /**
         * @copydoc underlying()
         */
        [[nodiscard]] const void* underlying() const noexcept {
            assert(pimpl_ && "Manager not initialized");
            return pimpl_->underlying();
        }

    };


}

