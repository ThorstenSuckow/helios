/**
 * @file DeadTagComponent.ixx
 * @brief Tag component marking an entity as dead.
 */
module;

#include <cassert>
#include <span>
#include <typeindex>
#include <memory>
#include <vector>

export module helios.gameplay.lifecycle.components.DeadTagComponent;



import helios.runtime.world.GameObject;
import helios.gameplay.spawn.types.SpawnProfileId;
import helios.ecs.types.ComponentTypeId;
import helios.core.types;

export namespace helios::gameplay::lifecycle::components {

    /**
     * @brief Marker component attached to entities whose health has been depleted.
     *
     * Systems can query for DeadTagComponent to apply death-related logic
     * (e.g. scoring, VFX, cleanup) without re-checking health values.
     * Move-only; copying is not permitted.
     */
    template<typename THandle>
    class DeadTagComponent{

    public:
        DeadTagComponent() = default;

        DeadTagComponent(const DeadTagComponent& other) = delete;
        DeadTagComponent& operator=(const DeadTagComponent& other) = delete;

        DeadTagComponent& operator=(DeadTagComponent&&) noexcept = default;
        DeadTagComponent(DeadTagComponent&&) noexcept = default;


    };

}