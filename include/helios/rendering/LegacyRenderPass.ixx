/**
 * @file LegacyRenderPass.ixx
 * @brief Abstraction representing a render pass (framebuffer, attachments, etc.).
 */
module;

#include <memory>
#include <cassert>

export module helios.rendering.LegacyRenderPass;

import helios.rendering.RenderQueue;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.viewport.Viewport;


import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::LegacyRenderPass"
export namespace helios::rendering {

    /**
     * @brief Encapsulates a single rendering pass with its associated resources.
     *
     * A `LegacyRenderPass` holds a `RenderQueue` containing all `MeshRenderCommand` and
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
    class LegacyRenderPass {

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
        const helios::rendering::viewport::Viewport* viewport_;

    protected:
        /**
         * @brief Shared logger instance for all LegacyRenderPass objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:
        ~LegacyRenderPass() = default;

        /**
         * @brief Prevent copying.
         * A LegacyRenderPass is not intended to be copied.
         */
        LegacyRenderPass(const LegacyRenderPass&) = delete;

        /**
         * @brief Prevent copy assignment.
         * A LegacyRenderPass is not intended to be copied.
         */
        LegacyRenderPass& operator=(const LegacyRenderPass&) = delete;

        /**
         * @brief Allow move.
         */
        LegacyRenderPass(LegacyRenderPass&&) noexcept = default;

        /**
         * @brief Allow move assignment.
         */
        LegacyRenderPass& operator=(LegacyRenderPass&&) noexcept = default;

        /**
         * @brief Creates a new `LegacyRenderPass` with the specified viewport, render queue, and frame uniforms.
         *
         * The render queue is moved into this pass. The viewport pointer must remain
         * valid for the lifetime of this pass.
         *
         * @param viewport Non-owning pointer to the viewport this LegacyRenderPass is processing.
         * @param renderQueue The `RenderQueue` to be processed with this pass (moved).
         * @param frameUniformValues Frame-specific uniform values (e.g., view/projection matrices).
         */
        explicit LegacyRenderPass(
            const helios::rendering::viewport::Viewport* viewport,
            helios::rendering::RenderQueue renderQueue,
            const helios::rendering::shader::UniformValueMap& frameUniformValues
            ) noexcept
            :
            viewport_(viewport),
            renderQueue_(std::move(renderQueue)),
            frameUniformValues_(frameUniformValues) {

            assert(viewport_ != nullptr && "Unexpected nullptr for viewport in LegacyRenderPass constructor");
        }

        /**
         * @brief Returns a const ref to the `RenderQueue` this `LegacyRenderPass` holds.
         *
         * @return A const ref to this `LegacyRenderPass`' `RenderQueue`.
         */
        [[nodiscard]] const RenderQueue& renderQueue() const noexcept {
            return renderQueue_;
        }

        /**
         * @brief Returns a const ref to the `Viewport` this `LegacyRenderPass` holds.
         *
         * @return A const ref to this `LegacyRenderPass`' `Viewport`.
         */
        [[nodiscard]] const helios::rendering::viewport::Viewport& viewport() const noexcept {
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
         * @brief Returns a const reference to this `LegacyRenderPass`' `UniformValueMap`.
         *
         * The map might be empty.
         *
         * @return A const reference to this `LegacyRenderPass`' `UniformValueMap` for the current frame.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& frameUniformValues() const noexcept {
            return frameUniformValues_;
        }
    };


}
