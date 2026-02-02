/**
 * @file MeshRenderCommand.ixx
 * @brief Representation of a low-level render command.
 */
module;

#include <memory>

export module helios.rendering.mesh.MeshRenderCommand;

import helios.rendering.RenderPrototype;
import helios.rendering.shader.UniformValueMap;

export namespace helios::rendering::mesh {

    /**
     * @brief DTO for storing rendering-specific command information to be passed to the RenderQueue.
     *
     * A `MeshRenderCommand` encapsulates all data needed to render a single mesh instance:
     * - A reference to the `RenderPrototype` (mesh + material)
     * - Object-specific uniform values (e.g., model matrix)
     * - Material-specific uniform values (e.g., colors, textures)
     *
     * ## Ownership Model
     *
     * - **RenderPrototype:** Non-owning raw pointer. The caller must ensure that the
     *   `RenderPrototype` remains valid for the lifetime of this command (typically
     *   within a single frame).
     * - **UniformValueMaps:** Owned by the command. Moved into the command on construction.
     *
     * ## Design
     *
     * - **Move-Only:** Commands are non-copyable to prevent accidental duplication
     *   during render queue processing.
     * - **Single-Frame Lifetime:** Commands are typically created during scene traversal
     *   and consumed by the renderer within the same frame.
     *
     * @see RenderQueue
     * @see MeshRenderer
     * @see RenderPrototype
     */
    class MeshRenderCommand {
        /**
         * @brief A non-owning raw pointer to the RenderPrototype to be used.
         *
         * The caller must ensure the `RenderPrototype` outlives this command.
         */
        const helios::rendering::RenderPrototype* renderPrototype_;

        /**
         * @brief An owning, unique pointer to the uniform values specific for the object to be rendered.
         * This map contains uniforms that change per object instance, such as the world
         * transformation matrix.
         */
        helios::rendering::shader::UniformValueMap objectUniformValues_;

        /**
         * @brief An owning, unique pointer to the uniform values specific to the material of the object to
         * be rendered. This map contains uniforms related to the surface properties of a material.
         */
        helios::rendering::shader::UniformValueMap materialUniformValues_;

        public:
        /**
         * @brief Prevent copying.
         * A MeshRenderCommand is not intended to be copied.
         */
        MeshRenderCommand(const MeshRenderCommand&)=delete;

        /**
         * @brief Prevent copy assignment.
         * A MeshRenderCommand is not intended to be copied.
         */
        MeshRenderCommand& operator=(const MeshRenderCommand&)=delete;

        /**
         * @brief Default move constructor.
         */
        MeshRenderCommand(MeshRenderCommand&&) noexcept = default;

        /**
         * @brief Default move assignment.
         */
        MeshRenderCommand& operator=(MeshRenderCommand&&) noexcept = default;

        /**
         * @brief Default destructor.
         */
        ~MeshRenderCommand() = default;

        /**
         * @brief Constructs a new `MeshRenderCommand`.
         *
         * Initializes this command with a non-owning pointer to the render prototype
         * and takes ownership of the provided `UniformValueMaps`.
         *
         * @param renderPrototype A raw pointer to the `RenderPrototype` to be associated with this command.
         *                        Must remain valid for the lifetime of this command.
         * @param objectUniformValues A `UniformValueMap` containing all uniform values for the rendered object.
         * @param materialUniformValues A `UniformValueMap` containing all uniform values for the material.
         */
        MeshRenderCommand(
            const helios::rendering::RenderPrototype* renderPrototype,
            const helios::rendering::shader::UniformValueMap& objectUniformValues,
            const helios::rendering::shader::UniformValueMap& materialUniformValues
            ) noexcept :
        renderPrototype_(renderPrototype),
        objectUniformValues_(objectUniformValues),
        materialUniformValues_(materialUniformValues) { }

        /**
         * @brief Returns the RenderPrototype associated with this command.
         *
         * @return A raw pointer to this command's RenderPrototype.
         *         May be `nullptr` if no prototype was provided.
         */
        [[nodiscard]] const helios::rendering::RenderPrototype* renderPrototype() const noexcept {
            return renderPrototype_;
        }

        /**
         * @brief Returns a const ref to this command's UniformValueMap for the object.
         *
         * @return A const ref to this command's UniformValueMap for the object.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& objectUniformValues() const noexcept {
            return objectUniformValues_;
        }

        /**
         * @brief Returns a const ref to this command's UniformValueMap for the material.
         *
         * @return A const ref to this command's UniformValueMap for the material.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& materialUniformValues() const noexcept {
            return materialUniformValues_;
        }
    };


}