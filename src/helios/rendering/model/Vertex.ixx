module;

export module helios.rendering.model:Vertex;

import helios.math.types;

export namespace helios::rendering::model {

    /**
     * @see [Vri20, pp. 160]
     */
    struct Vertex {
        math::vec3 position;
        math::vec3 normal;
    };


}
