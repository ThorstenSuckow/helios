/**
 * @file RenderPass.ixx
 * @brief Abstraction representing a render pass (framebuffer, attachments, etc.).
 */
module;

#include <memory>
#include <cassert>

export module helios.rendering.RenderPass;

import helios.rendering.RenderQueue;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.Viewport;


import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderPass"
export namespace helios::rendering {

    /**
     * @brief Represents a single RenderPass under a set of specific uniform values.
     *
     * A `RenderPass` holds a `RenderQueue` that contains all `RenderCommands` that need
     * to be processed in the subsequent rendering. The associated uniform values specified
     * with this `RenderPass` remain constant during processing of the `RenderCommand`s.
     *
     * `RenderPass`es are typically processed sequentially by an API-specific implementation
     * of the pure virtual `RenderingDevice` class.
     *
     * @note A `RenderPass` should be configurable with other properties relevant to the underlying
     * rendering backend, e.g. depth testing, draw mode etc.
     */
    class RenderPass {

    private:
        /**
         * @brief An owning unique pointer to the `RenderQueue` for this pass.
         */
        std::unique_ptr<helios::rendering::RenderQueue> renderQueue_;

        /**
         * @brief An owning unique pointer to the uniform values specific to the current pass (i.e.,
         * the rendered "frame"). This map contains uniforms that change once per frame, such as the view and the
         * projection matrix.
         */
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues_;

        /**
         * @brief Shared ownership of the `Viewport` processed by this pass.
         */
        std::shared_ptr<const helios::rendering::Viewport> viewport_;

    protected:
        /**
         * @brief Shared logger instance for all RenderPass objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:
        ~RenderPass() = default;

        /**
         * @brief Prevent copying.
         * A RenderPass is not intended to be copied.
         */
        RenderPass(const RenderPass&) = delete;

        /**
         * @brief Prevent copy assignment.
         * A RenderPass is not intended to be copied.
         */
        RenderPass& operator=(const RenderPass&) = delete;

        /**
         * @brief Prevent move.
         */
        RenderPass(RenderPass&&) noexcept = delete;

        /**
         * @brief Prevent move assignment.
         */
        RenderPass& operator=(RenderPass&&) noexcept = delete;

        /**
         * @brief Create a new `RenderPass` with the specified  `Viewport`, `RenderQueue` and the `UniformValueMap`.
         * Ownership of Viewport is shared with this instance, the latter objects are transferred to **this** `RenderPass`.
         *
         * @param viewport The viewport this RenderPass is processing.
         * @param renderQueue A unique ptr to the `RenderQueue` that should be processed with this pass.
         * @param frameUniformValues A unique ptr to the `UniformValueMap` associated with this pass, i.e.
         * frame-specific uniform values. If no values are specified, a default empty set is created.
         */
        explicit RenderPass(
            std::shared_ptr<const helios::rendering::Viewport> viewport,
            std::unique_ptr<helios::rendering::RenderQueue> renderQueue,
            std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues = nullptr
            ) noexcept
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

        /**
         * @brief Returns a const ref to the `RenderQueue` this `RenderPass` holds.
         *
         * @return A const ref to this `RenderPass`' `RenderQueue`.
         */
        [[nodiscard]] const RenderQueue& renderQueue() const noexcept {
            return *renderQueue_;
        }

        /**
         * @brief Returns a const ref to the `Viewport` this `RenderPass` holds.
         *
         * @return A const ref to this `RenderPass`' `Viewport`.
         */
        [[nodiscard]] const Viewport& viewport() const noexcept {
            return *viewport_;
        }

        /**
         * @brief Sets this `RenderPass`' `UniformValueMap` containing per-frame specific uniform values.
         * Transfers ownership of the provided UniformValueMap to this `RenderPass`.
         *
         * @param frameUniformValues A unique ptr to the `UniformValueMap` for frame specific uniform values.
         */
        void setFrameUniformValues(std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues) noexcept {
            frameUniformValues_ = std::move(frameUniformValues);
        }

        /**
         * @brief Returns a const reference to this `RenderPass`' `UniformValueMap`.
         *
         * The map might be empty.
         *
         * @return A const reference to this `RenderPass`' `UniformValueMap` for the current frame.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& frameUniformValues() const noexcept {
            return *frameUniformValues_;
        }
    };


}
