/**
 * @brief Data-transfer object describing a single render command.
 */
module;

#include <memory>

export module helios.rendering.RenderCommand;

import helios.rendering.model.Mesh;
import helios.rendering.shader.Shader;
import helios.rendering.shader.UniformValueMap;

export namespace helios::rendering {

    /**
     * @brief DTO for storing rendering-specific command information to be
     * passed to the RenderQueue.
     *
     * A `RenderCommand` does not extend the lifetime of the `shader_` and the `mesh_`,
     * as they are both referenced by weak pointers. If the referenced shader or the mesh
     * are no longer valid when this command is about to be executed,
     * this `RenderCommand` should be ignored for rendering.
     */
    class RenderCommand {
        /**
         * @brief A non-owning, weak reference to the shader program to be used.
         */
        std::weak_ptr<const helios::rendering::shader::Shader> shader_;

        /**
         * @brief A non-owning, weak reference to the mesh this command uses.
         */
        std::weak_ptr<const helios::rendering::model::Mesh> mesh_;

        /**
         * @brief An owning, unique pointer to the uniform values specific for the object to be rendered.
         * This map contains uniforms that change per object instance, such as the world
         * transformation matrix.
         */
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> objectUniformValues_;

        /**
         * @brief An owning, unique pointer to the uniform values specific to the material of the object to
         * be rendered. This map contains uniforms related to the surface properties of a material.
         */
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> materialUniformValues_;

        public:
        /**
         * @brief Prevent copying.
         * A RenderCommand is not intended to be copied.
         */
        RenderCommand(const RenderCommand&)=delete;

        /**
         * @brief Prevent copy assignment.
         * A RenderCommand is not intended to be copied.
         */
        RenderCommand& operator=(const RenderCommand&)=delete;

        /**
         * @brief Delete move constructor.
         */
        RenderCommand(RenderCommand&&) noexcept = delete;

        /**
         * @brief Delete move assignment.
         */
        RenderCommand& operator=(RenderCommand&&) noexcept = delete;

        ~RenderCommand() = default;

        /**
         * @brief Constructs a new `RenderCommand`.
         * Initializes this `RenderCommand` with weak pointers to the shader and mesh, and
         * takes ownership of the provided `UniformValueMaps`.
         *
         * @param shader A weak_ptr to the `Shader` to be associated with this command.
         * @param mesh A weak_ptr to the `Mesh` to be associated with this command.
         * @param objectUniformValues A unique ptr to a `UniformValueMap` containing all uniform
         * values for the rendered object.
         * @param materialUniformValues A unique ptr to a `UniformValueMap` containing all uniform
         * values for the material.
         */
        RenderCommand(
        std::weak_ptr<const helios::rendering::shader::Shader> shader,
        std::weak_ptr<const helios::rendering::model::Mesh> mesh,
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> objectUniformValues,
        std::unique_ptr<const helios::rendering::shader::UniformValueMap> materialUniformValues
            ) noexcept;

        /**
         * @brief Returns a weak ptr to the shader associated with this command.
         * Use `shader.lock()` on the return value to lock the shader into a shared ptr.
         * If `lock()` returns a nullptr, the Shader is no longer available.
         *
         * @return A weak ptr to this command's Shader.
         */
        [[nodiscard]] std::weak_ptr<const helios::rendering::shader::Shader> shader() const noexcept;

        /**
         * @brief Returns a weak ptr to the mesh associated with this command.
         * Use `mesh.lock()` on the return value to lock the mesh into a shared ptr.
         * If `lock()` returns a nullptr, the Mesh is no longer available.
         *
         * @return A weak ptr to this command's Mesh.
         */
        [[nodiscard]] std::weak_ptr<const helios::rendering::model::Mesh> mesh() const noexcept;

        /**
         * @brief Returns a const ref to this command's UniformValueMap for the object.
         *
         * @return A const ref to this command's UniformValueMap for the object.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& objectUniformValues() const noexcept;

        /**
         * @brief Returns a const ref to this command's UniformValueMap for the material.
         *
         * @return A const ref to this command's UniformValueMap for the material.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& materialUniformValues() const noexcept;
    };


}