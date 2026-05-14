/**
 * @file CameraBindingComponent.ixx
 * @brief Binding component alias for camera game objects.
 */
module;

export module helios.scene.components.CameraBindingComponent;

import helios.runtime.world.types.GameObjectHandle;

import helios.ecs.components.BindingComponent;

using namespace helios::runtime::world::types;
using namespace helios::ecs::components;
export namespace helios::scene::components {

    /**
     * @brief Binds an owning entity to a camera game object handle.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using CameraBindingComponent = BindingComponent<TOwnerHandle, GameObjectHandle>;

}