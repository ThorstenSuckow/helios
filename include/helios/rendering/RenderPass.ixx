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
     * @brief Encapsulates a single rendering pass with its associated resources.
     *
     * A `RenderPass` holds a `RenderQueue` containing all `MeshRenderCommand` and
     * `TextRenderCommand` objects to be processed. It also stores frame-level
     * uniform values (e.g., view and projection matrices) that remain constant
     * during the pass.
     *
     * ## Components
     *
     * - **RenderQueue:** Contains geometry and text render commands.
     * - **Viewport:** Non-owning pointer to the rendering area and clear settings.
     * - **UniformValueMap:** Frame-level uniforms applied to all commands.
     *
     * ## Ownership Model
     *
     * - **RenderQueue:** Owned by value (moved into the pass).
     * - **UniformValueMap:** Owned by value (copied/moved into the pass).
     * - **Viewport:** Non-owning raw pointer. The caller must ensure the viewport
     *   remains valid for the lifetime of this pass.
     *
     * ## Lifecycle
     *
     * ```
     * RenderingDevice::beginRenderPass(pass)
     *     → Clear buffers, configure viewport
     * RenderingDevice::doRender(pass)
     *     → Process MeshRenderCommands and TextRenderCommands
     * RenderingDevice::endRenderPass(pass)
     *     → Finalize pass, unbind resources
     * ```
     *
     * @note Future versions should support additional configuration like depth
     *       testing, stencil operations, and draw modes.
     *
     * @see RenderQueue
     * @see RenderingDevice
     * @see Viewport
     * @see RenderPassFactory
     */
    class RenderPass {

    private:
        /**
         * @brief The `RenderQueue` for this pass, containing all render commands.
         */
        helios::rendering::RenderQueue renderQueue_;

        /**
         * @brief Uniform values specific to the current frame.
         *
         * This map contains uniforms that change once per frame, such as the view
         * and the projection matrices.
         */
        helios::rendering::shader::UniformValueMap frameUniformValues_;

        /**
         * @brief Non-owning pointer to the `Viewport` processed by this pass.
         *
         * The caller must ensure the viewport remains valid for the lifetime of this pass.
         */
        const helios::rendering::Viewport* viewport_;

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
         * @brief Allow move.
         */
        RenderPass(RenderPass&&) noexcept = default;

        /**
         * @brief Allow move assignment.
         */
        RenderPass& operator=(RenderPass&&) noexcept = default;

        /**
         * @brief Creates a new `RenderPass` with the specified viewport, render queue, and frame uniforms.
         *
         * The render queue is moved into this pass. The viewport pointer must remain
         * valid for the lifetime of this pass.
         *
         * @param viewport Non-owning pointer to the viewport this RenderPass is processing.
         * @param renderQueue The `RenderQueue` to be processed with this pass (moved).
         * @param frameUniformValues Frame-specific uniform values (e.g., view/projection matrices).
         */
        explicit RenderPass(
            const helios::rendering::Viewport* viewport,
            helios::rendering::RenderQueue renderQueue,
            const helios::rendering::shader::UniformValueMap& frameUniformValues
            ) noexcept
            :
            viewport_(viewport),
            renderQueue_(std::move(renderQueue)),
            frameUniformValues_(frameUniformValues) {}

        /**
         * @brief Returns a const ref to the `RenderQueue` this `RenderPass` holds.
         *
         * @return A const ref to this `RenderPass`' `RenderQueue`.
         */
        [[nodiscard]] const RenderQueue& renderQueue() const noexcept {
            return renderQueue_;
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
         * @brief Sets the frame-specific uniform values for this pass.
         *
         * @param frameUniformValues The `UniformValueMap` containing frame-specific uniform values.
         */
        void setFrameUniformValues(const helios::rendering::shader::UniformValueMap& frameUniformValues) noexcept {
            frameUniformValues_ = frameUniformValues;
        }

        /**
         * @brief Returns a const reference to this `RenderPass`' `UniformValueMap`.
         *
         * The map might be empty.
         *
         * @return A const reference to this `RenderPass`' `UniformValueMap` for the current frame.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& frameUniformValues() const noexcept {
            return frameUniformValues_;
        }
    };


}
