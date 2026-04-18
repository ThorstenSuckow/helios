module;



export module helios.rendering.commands.RenderCommand;

import helios.scene.types.SceneMemberRenderContext;

using namespace helios::scene::types;
export namespace helios::rendering::commands {


    template<typename THandle>
    struct RenderCommand {

        const SceneMemberRenderContext<THandle> sceneMemberRenderContext;

    };


}

