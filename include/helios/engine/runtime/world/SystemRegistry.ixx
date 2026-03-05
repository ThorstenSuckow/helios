/**
 * @file SystemRegistry.ixx
 * @brief Type-indexed registry for managing System instances within a game loop pass.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>


export module helios.engine.runtime.world.SystemRegistry;

import helios.engine.core.data;
import helios.engine.runtime.world.System;

using namespace helios::engine::core::data;

export namespace helios::engine::runtime::world {

    /**
     * @brief Type-indexed registry for managing System instances within a game loop pass.
     *
     * @details SystemRegistry owns System<T> wrappers internally and
     * provides O(1) access to the underlying concrete system via
     * SystemTypeId. Systems are registered once via `add<T>()` and then
     * iterated each frame through the `systems()` span.
     *
     * @see System
     * @see System
     * @see SystemTypeId
     */
    class SystemRegistry {

        /**
         * @brief Owning storage for wrapped System instances, indexed by SystemTypeId.
         */
        std::vector<System> systems_;

        /**
         * @brief Raw pointers to the underlying T instances for O(1) typed access.
         */
        std::vector<void*> underlyingSystems_;

        /**
         * @brief Stores the insertion order for the systems.
         */
        std::vector<size_t> insertionOrder_;

        std::vector<System*> systemsView_;


        bool needsUpdate_ = false;

        void update() {

            if (!needsUpdate_) {
                return;
            }

            systemsView_.clear();
            systemsView_.reserve(insertionOrder_.size());

            for (const auto insertionIndex : insertionOrder_) {
                systemsView_.push_back(&systems_[insertionIndex]);
            }

        }


    public:

        /**
         * @brief Returns a span of all registered systems for iteration.
         *
         * @return Span of System pointers.
         */
        [[nodiscard]] std::span<System* const> systems() noexcept {
            update();
            return systemsView_;
        }


        template<typename T, typename... Args>
        requires helios::engine::runtime::world::HasUpdate<T>
        T& add(Args&&... args) {

            assert(!hasSystem<T>() && "System already registered with GameLoopPhase");

            System system{T{std::forward<Args>(args)...}};

            const auto idx = SystemTypeId::id<T>().value();

            if (systems_.size() <= idx) {
                systems_.resize(idx + 1);
            }
            if (underlyingSystems_.size() <= idx) {
                underlyingSystems_.resize(idx + 1);
            }

            void* rawUnderlying = system.underlying();

            underlyingSystems_[idx] = rawUnderlying;
            systems_[idx] = std::move(system);
            insertionOrder_.push_back(idx);

            needsUpdate_ = true;
            return *static_cast<T*>(rawUnderlying);
        }

        /**
         * @brief Checks whether a system of type T is registered.
         *
         * @tparam T The system type to check.
         *
         * @return True if registered, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool hasSystem() const {
            return getSystem<T>() != nullptr;
        }

        /**
         * @brief Returns a pointer to the underlying system of type T.
         *
         * @tparam T The system type to retrieve.
         *
         * @return Pointer to the system, or nullptr if not registered.
         */
        template<typename T>
        [[nodiscard]] T* getSystem() const {

            const auto idx = SystemTypeId::id<T>().value();
            if (systems_.size() <= idx || underlyingSystems_.size() <= idx) {
                return nullptr;
            }

            return underlyingSystems_[idx] ? static_cast<T*>(underlyingSystems_[idx]) : nullptr;
        }

    };


}