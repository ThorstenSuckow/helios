module;

export module helios.rendering.core.Vertex;

import helios.math.types;

export namespace helios::rendering::core {

    /**
     * Vertex data structure for maintaining
     * position, normal and texture coordinates.
     *
     * @see [Vri20, pp. 160]
     */
    struct Vertex {
        math::vec3 position;
        math::vec3 normal;
        math::vec2 texCoords;
    };


}
