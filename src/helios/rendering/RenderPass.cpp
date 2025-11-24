module;

#include <cassert>
#include <memory>

module helios.rendering.RenderPass;

import helios.rendering.RenderQueue;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.Viewport;

namespace helios::rendering {


    RenderPass::RenderPass(
        std::shared_ptr<const helios::rendering::Viewport> viewport,
        std::unique_ptr<helios::rendering::RenderQueue> renderQueue,
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues) noexcept
        :
        viewport_(std::move(viewport)),
        renderQueue_(std::move(renderQueue)),
        frameUniformValues_(std::move(frameUniformValues)) {

        if (!viewport_) {
            logger_.error("Constructor received a nullptr Viewport. Creating a default empty Viewport.");
            viewport_ = std::make_shared<Viewport>();
        }

        if (!renderQueue_) {
            logger_.error("Constructor received a nullptr RenderQueue. Creating a default empty queue.");
            renderQueue_ = std::make_unique<helios::rendering::RenderQueue>();
        }
        
        if (!frameUniformValues_) {
            logger_.warn("Constructor received a nullptr UniformValueMap. Creating a default empty map.");
            frameUniformValues_ = std::make_unique<helios::rendering::shader::UniformValueMap>();
        }
    }

    [[nodiscard]] const Viewport& RenderPass::viewport() const noexcept {
        return *viewport_;
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

