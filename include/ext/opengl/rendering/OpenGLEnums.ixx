module;

#include <glad/gl.h>
#include <cassert>

export module helios.ext.opengl.rendering.OpenGLEnumMapper;

import helios.rendering.mesh.PrimitiveType;

export namespace helios::ext::opengl::rendering::OpenGLEnumMapper {

    /**
     * @brief Translates a helios abstract PrimitiveType enum to its corresponding OpenGL GLenum value.
     *
     * This utility function maps API-agnostic primitive types defined by helios to their
     * OpenGL equivalents. It is used internally by OpenGLMeshRenderer and other OpenGL
     * rendering components.
     *
     * Supported mappings:
     * - `PrimitiveType::Points` → `GL_POINTS`
     * - `PrimitiveType::Lines` → `GL_LINES`
     * - `PrimitiveType::LineLoop` → `GL_LINE_LOOP`
     * - `PrimitiveType::LineStrip` → `GL_LINE_STRIP`
     * - `PrimitiveType::Triangles` → `GL_TRIANGLES`
     * - `PrimitiveType::TriangleStrip` → `GL_TRIANGLE_STRIP`
     * - `PrimitiveType::TriangleFan` → `GL_TRIANGLE_FAN`
     *
     * Example usage:
     * ```cpp
     * auto glPrimitive = OpenGLEnumMapper::toOpenGL(PrimitiveType::Triangles);
     * glDrawElements(glPrimitive, indexCount, GL_UNSIGNED_INT, nullptr);
     * ```
     *
     * @param primitiveType The API-agnostic PrimitiveType value.
     *
     * @return The corresponding OpenGL primitive type as a GLenum. Falls back to
     *         `GL_TRIANGLES` if the mapping is not explicitly defined.
     *
     * @note This function is marked `[[nodiscard]]` to encourage proper usage of the return value.
     */
    [[nodiscard]] GLenum toOpenGL(helios::rendering::mesh::PrimitiveType primitiveType) noexcept {
        switch (primitiveType) {
            case helios::rendering::mesh::PrimitiveType::Points:
                return GL_POINTS;
            case helios::rendering::mesh::PrimitiveType::Lines:
                return GL_LINES;
            case helios::rendering::mesh::PrimitiveType::LineLoop:
                return GL_LINE_LOOP;
            case helios::rendering::mesh::PrimitiveType::LineStrip:
                return GL_LINE_STRIP;
            case helios::rendering::mesh::PrimitiveType::Triangles:
                return GL_TRIANGLES;
            case helios::rendering::mesh::PrimitiveType::TriangleStrip:
                return GL_TRIANGLE_STRIP;
            case helios::rendering::mesh::PrimitiveType::TriangleFan:
                return GL_TRIANGLE_FAN;
            default:
                return GL_TRIANGLES;
        }
    }
}