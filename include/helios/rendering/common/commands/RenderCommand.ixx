/**
 * @file RenderCommand.ixx
 * @brief Command wrapper for extracted scene member render work.
 */
module;



export module helios.rendering.common.commands.RenderCommand;

import helios.scene.types.SceneMemberRenderContext;

using namespace helios::scene::types;
export namespace helios::rendering::common::commands {


    /**
     * @brief Command that carries one scene-member render context.
     *
     * @tparam THandle Scene member handle type.
     */
    template<typename THandle>
    struct RenderCommand {

        /** @brief Render context for the submitted scene member. */
        const SceneMemberRenderContext<THandle> sceneMemberRenderContext;

    };


}

