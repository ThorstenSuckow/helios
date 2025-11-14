module;

#include <memory>

module helios.rendering.RenderCommand;

import helios.rendering.RenderPrototype;
import helios.rendering.shader.UniformValueMap;

namespace helios::rendering {

    using namespace helios::rendering::shader;

    RenderCommand::RenderCommand(
    std::weak_ptr<const RenderPrototype> renderPrototype,
    std::unique_ptr<const UniformValueMap> objectUniformValues,
    std::unique_ptr<const UniformValueMap> materialUniformValues
        ) noexcept :
    renderPrototype_(std::move(renderPrototype)),
    objectUniformValues_(std::move(objectUniformValues)),
    materialUniformValues_(std::move(materialUniformValues)) { }

    std::weak_ptr<const RenderPrototype> RenderCommand::renderPrototype() const noexcept {
        return renderPrototype_;
    }


    const UniformValueMap& RenderCommand::objectUniformValues() const noexcept {
        return *objectUniformValues_;
    }

    const UniformValueMap& RenderCommand::materialUniformValues() const noexcept {
        return *materialUniformValues_;
    }
    
}