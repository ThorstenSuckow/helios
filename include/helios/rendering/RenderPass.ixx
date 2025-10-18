module;

#include <memory>

export module helios.rendering.RenderPass;

import helios.rendering.RenderQueue;
import helios.rendering.NullRenderQueue;
import helios.rendering.shader.UniformValueMap;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderPass"
export namespace helios::rendering {

    /**
     * Represents a single RenderPass under a set of specific uniform values.
     *
     * A `RenderPass` holds a `RenderQueue` that contains all `RenderCommands` that need
     * to be processed in the subsequent rendering.
     * The associated uniform values specified with this `RenderPass`remain constant
     * during processing the `RenderCommmand`s.
     *
     * `RenderPass`es are typically processed sequentially by an API-specific implementation
     * of the pure virtual `RenderingDevice`-class.
     */
    class RenderPass {

    private:
        /**
         * An owning, unique pointer to the `RenderQueue` for this pass.
         *
         */
        std::unique_ptr<helios::rendering::RenderQueue> renderQueue_;

        /**
         * An owning, unique pointer to the uniform values specific to the current pass (i.e.,
         * the rendered "frame").
         * This map contains uniforms that change once per frame, such as the view and the
         * projection matrix.
         */
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues_;

    protected:
        /**
         * The logger used with this Scene instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        ~RenderPass() = default;

        /**
         * Prevent copying.
         * A RenderPass is not intended to be copied.
         */
        RenderPass(const RenderPass&) = delete;

        /**
         * Prevent Copying.
         * A RenderPass is not intended to be copied.
         */
        RenderPass& operator=(const RenderPass&) = delete;

        /**
         * Prevent move.
         */
        RenderPass(RenderPass&&) noexcept = delete;

        /**
         * Prevent move assignment.
         */
        RenderPass& operator=(RenderPass&&) noexcept = delete;

        /**
         * Create a new `RenderPass` with the specified `RenderQueue` and the `UniformValueMap`.
         * Ownership of both objects is transferred to **this** `RenderPass`.
         *
         * @param renderQueue A unique ptr to the `RenderQueue` that should be processes with this pass.
         * If a nullptr was submitted, an implicit default NullRenderQueue is created.
         * @param frameUniformValues A unique ptr to the UniformValueMap associated with this pass, i.e.
         * frame specific Uniform Values. If no values are specified, a default empty set is created.
         */
        explicit RenderPass(
            std::unique_ptr<helios::rendering::RenderQueue> renderQueue,
            std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues = nullptr
            ) noexcept;

        /**
         * Returns a const ref to the `RenderQueue` this `RenderPass` holds.
         * Might be a `NullRenderQueue` if the caller of the constructor did not pass a valid
         * `RenderQueue`
         *
         * @ return A const ref to this `RenderPass` `RenderQueue`, which might be a `NullRenderQueue.`
         */
        [[nodiscard]] const RenderQueue& renderQueue() const noexcept;

        /**
         * Sets this `RenderPass`' `UniformValueMap` containing per-frame specific uniform values.
         * Transfers ownership of the provided UniformValueMap to this `RenderPass`.
         *
         * @param frameUniformValues A unique ptr to the `UniformValueMap` for frame specific uniform values.
         */
        void setFrameUniformValues(std::unique_ptr<const helios::rendering::shader::UniformValueMap> frameUniformValues) noexcept;

        /**
         * Returns a const ref to this `RenderPass`' `UniformValueMap`.
         * The map might be empty.
         *
         * @return A const ref to this RenderPass' UniformValueMap for the frame.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& frameUniformValues() const noexcept;
    };


}

