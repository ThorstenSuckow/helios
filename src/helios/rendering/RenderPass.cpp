module;

#include <cassert>
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
        assert(renderQueue_ && "Received nullptr RenderQueue.");
        if (!renderQueue_) {
            logger_.error("Constructor received a nullptr RenderQueue.");
        }
        if (frameUniformValues_ == nullptr) {
            logger_.warn("Constructor received a nullptr UniformValueMap.");
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

