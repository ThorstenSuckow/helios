module;

#include <memory>

module helios.rendering.RenderPass;

import helios.rendering.RenderQueue;
import helios.rendering.shader.UniformValueMap;

namespace helios::rendering {


    RenderPass::RenderPass(
        std::unique_ptr<helios::rendering::RenderQueue> renderQueue,
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues) noexcept
        :
        renderQueue_(std::move(renderQueue)),
        frameUniformValues_(std::move(frameUniformValues)) {
        if (!renderQueue_) {
            renderQueue_ = std::make_unique<helios::rendering::NullRenderQueue>();
        }
        if (frameUniformValues_ == nullptr) {
            frameUniformValues_ = std::make_unique<helios::rendering::shader::UniformValueMap>();
        }
    }


    [[nodiscard]] const RenderQueue& RenderPass::renderQueue() const noexcept {
        return *renderQueue_;
    }


    void RenderPass::setFrameUniformValues(std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues) noexcept{
        frameUniformValues_ = std::move(frameUniformValues);
    }


    [[nodiscard]] const helios::rendering::shader::UniformValueMap& RenderPass::frameUniformValues() const noexcept{
        return *frameUniformValues_;
    }

}

