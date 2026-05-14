/**
 * @file SceneMemberComponent.ixx
 * @brief Binding component alias for scene membership.
 */
module;


export module helios.scene.components.SceneMemberComponent;

import helios.scene.types.SceneHandle;

import helios.ecs.components.BindingComponent;

using namespace helios::scene::types;
using namespace helios::ecs::components;
export namespace helios::scene::components {

    /**
     * @brief Marks an owning entity as a member of a scene.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using SceneMemberComponent = BindingComponent<TOwnerHandle, SceneHandle>;

}