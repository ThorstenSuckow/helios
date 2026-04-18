module;



export module helios.engine.modules.rendering.commands.RenderCommand;

import helios.scene.types.SceneMemberRenderContext;

using namespace helios::scene::types;
export namespace helios::engine::modules::rendering::commands {


    template<typename THandle>
    struct RenderCommand {

        const SceneMemberRenderContext<THandle> sceneMemberRenderContext;

    };


}

