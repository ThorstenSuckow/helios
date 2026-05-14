/**
 * @file SceneBindingComponent.ixx
 * @brief Binding component alias for scene handles.
 */
module;


export module helios.scene.components.SceneBindingComponent;

import helios.scene.types.SceneHandle;
import helios.ecs.components.BindingComponent;

using namespace helios::scene::types;
using namespace helios::ecs::components;
export namespace helios::scene::components {

    /**
     * @brief Binds an owning entity to a scene handle.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using SceneBindingComponent = BindingComponent<TOwnerHandle, SceneHandle>;

}