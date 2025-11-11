/**
 * @file Vertex.ixx
 * @brief Vertex description and utilities for rendering.
 */
module;

export module helios.rendering.Vertex;

import helios.math.types;

export namespace helios::rendering {

    /**
     * @brief Represents a standard vertex with position, normal,
     * and texture coordinates.
     *
     * @see [Vri20, pp. 160]
     */
    struct Vertex {
        math::vec3f position;
        math::vec3f normal = {0.0f, 0.0f, 0.0f};
        math::vec2f texCoords = {0.0f, 0.0f};
    };
}
