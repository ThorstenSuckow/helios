module;

#include <memory>

module helios.rendering.RenderCommand;

import helios.rendering.model.Mesh;
import helios.rendering.shader.Shader;
import helios.rendering.shader.UniformValueMap;

using namespace helios::rendering::shader;
using namespace helios::rendering::model;

namespace helios::rendering {

    RenderCommand::RenderCommand(
    std::weak_ptr<const Shader> shader,
    std::weak_ptr<const Mesh> mesh,
    std::unique_ptr<const UniformValueMap> objectUniformValues,
    std::unique_ptr<const UniformValueMap> materialUniformValues
        ) noexcept :
    shader_(std::move(shader)),
    mesh_(std::move(mesh)),
    objectUniformValues_(std::move(objectUniformValues)),
    materialUniformValues_(std::move(materialUniformValues)) { }

    std::weak_ptr<const Shader> RenderCommand::shader() const noexcept {
        return shader_;
    }

    std::weak_ptr<const Mesh> RenderCommand::mesh() const noexcept {
        return mesh_;
    }

    const UniformValueMap& RenderCommand::objectUniformValues() const noexcept {
        return *objectUniformValues_;
    }

    const UniformValueMap& RenderCommand::materialUniformValues() const noexcept {
        return *materialUniformValues_;
    }
    
}