/**
 * @file Renderable.ixx
 * @brief Representative of a configurable Renderable rendered by the underlying GL API.
 */
module;

#include <memory>
#include <optional>
#include <stdexcept>

export module helios.rendering.Renderable;

import helios.rendering.RenderQueue;
import helios.rendering.RenderPrototype;
import helios.rendering.model.config.MaterialPropertiesOverride;
import helios.rendering.shader.UniformValueMap;

import helios.util.log.Logger;
import helios.util.log.LogManager;

import helios.math;

#define HELIOS_LOG_SCOPE "helios::rendering::Renderable"
export namespace helios::rendering {

    /**
     * @brief Abstract base class for objects that can be rendered by the rendering system.
     *
     * A `Renderable` represents any visual entity that can submit render commands to the
     * rendering pipeline. Concrete implementations define how the object emits itself
     * to a `RenderQueue` and how uniform values are written for shader consumption.
     *
     * Subclasses must implement:
     * - `localAABB()`: Returns the local-space axis-aligned bounding box.
     * - `writeUniformValues()`: Writes object-specific uniform values to a uniform map.
     * - `emit()`: Submits render commands to the render queue.
     *
     * @see helios::rendering::RenderQueue
     * @see helios::rendering::RenderPrototype
     */
    class Renderable {

    protected:


    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~Renderable() = default;

        /**
         * @brief Default constructor.
         */
        Renderable() = default;

        /**
         * @brief Deleted copy constructor.
         *
         * Renderables are non-copyable to prevent accidental duplication of GPU resources.
         */
        Renderable(const Renderable&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         *
         * Renderables are non-copyable to prevent accidental duplication of GPU resources.
         */
        Renderable& operator=(const Renderable&)= delete;

        /**
         * @brief Move constructor.
         *
         * Allows transfer of ownership of rendering resources.
         */
        Renderable(Renderable&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         *
         * Allows transfer of ownership of rendering resources.
         */
        Renderable& operator=(Renderable&&) noexcept = default;

        /**
         * @brief Returns the local-space axis-aligned bounding box of this renderable.
         *
         * The AABB is used for visibility culling, collision detection, and spatial queries.
         * It represents the bounds of the renderable in its local coordinate system before
         * any world transformations are applied.
         *
         * @return A const reference to the local-space AABB.
         */
        [[nodiscard]] virtual const helios::math::aabbf& localAABB() const noexcept = 0;

        /**
         * @brief Writes object-specific uniform values to a uniform value map.
         *
         * This method allows the renderable to contribute its own uniform values
         * (such as material properties or custom shader parameters) to the rendering
         * pipeline.
         *
         * @param uniformValueMap The uniform value map to write values to.
         */
        virtual void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept = 0;

        /**
         * @brief Emits render commands to the render queue.
         *
         * This method is called during the render traversal to submit this renderable's
         * draw commands to the render queue. The render queue collects and potentially
         * sorts these commands for efficient GPU execution.
         *
         * @param renderQueue The render queue to emit commands to.
         * @param objectUniformValues Uniform values specific to this object instance
         *        (e.g., model matrix, object ID).
         * @param materialUniformValues Uniform values for the material properties
         *        (e.g., colors, textures, shader parameters).
         */
        virtual void emit(
            RenderQueue& renderQueue,
            helios::rendering::shader::UniformValueMap objectUniformValues,
            helios::rendering::shader::UniformValueMap  materialUniformValues) const = 0;

    };

}