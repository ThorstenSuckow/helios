/**
 * @file RuntimeEnvironment.ixx
 * @brief Runtime environment facade for platform readiness state.
 */
module;

#include <optional>
#include <vector>
#include <cassert>

export module helios.runtime.world.RuntimeEnvironment;

import helios.ecs.Entity;
import helios.ecs.types.EntityHandle;

import helios.platform.environment.types;
import helios.platform.environment.components;

import helios.platform.environment.PlatformEntityManager;

using namespace helios::ecs;
using namespace helios::platform::environment;
using namespace helios::platform::environment::types;
using namespace helios::platform::environment::components;

export namespace helios::runtime::world {


    /**
     * @brief Facade around the platform-environment entity used by runtime systems.
     */
    class RuntimeEnvironment {

    private:

        using PlatformEntity = Entity<PlatformEntityManager>;

        using Handle_type = typename PlatformEntity::Handle_type;

        /**
         * @brief The underlying Entity storing system components.
         */
        PlatformEntity entity_;

    
    public:

        /**
         * @brief Constructs a runtime-environment facade from an existing platform entity.
         *
         * @param entity Platform environment entity.
         */
        explicit RuntimeEnvironment(const PlatformEntity& entity) : entity_(entity) {}

        /**
         * @brief Checks whether GPU context readiness was marked.
         *
         * @return `true` if GPU context is ready, otherwise `false`.
         */
        [[nodiscard]] bool isGPUReady() const noexcept {
            return entity_.has<GPUContextReadyComponent<Handle_type>>();
        }

        /**
         * @brief Marks the runtime environment as initialized.
         *
         * @return `true` if initialization marker was added, otherwise `false`.
         */
        [[nodiscard]] bool initialize() noexcept {
            if (isInitialized()) {
                assert(false && "RuntimeEnvironment already initialized");
                return false;
            }
            entity_.add<PlatformInitializedComponent<Handle_type>>();
            return true;
        }

        /**
         * @brief Checks whether initialization marker is present.
         *
         * @return `true` if initialized, otherwise `false`.
         */
        [[nodiscard]] bool isInitialized() const noexcept {
            return entity_.has<PlatformInitializedComponent<Handle_type>>();
        }

        /**
         * @brief Checks whether audio backend is ready.
         *
         * @return `true` when audio is considered ready.
         */
        [[nodiscard]] bool isAudioReady() const noexcept {
            return true;
        }

        /**
         * @brief Marks GPU context readiness.
         */
        void setGPUReady() noexcept {
            assert(!isGPUReady() && "GPUContextReadyComponent already set");
            entity_.add<GPUContextReadyComponent<Handle_type>>();
        }

        /**
         * @brief Checks whether all runtime-infrastructure prerequisites are ready.
         *
         * @return `true` if runtime infrastructure is ready, otherwise `false`.
         */
        [[nodiscard]] bool isRuntimeInfrastructureReady() const noexcept {
            return isGPUReady() && isAudioReady();;
        }
            
    };


}