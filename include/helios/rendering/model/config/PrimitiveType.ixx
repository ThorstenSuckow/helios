/**
 * @file PrimitiveType.ixx
 * @brief Enum of primitive types for mesh rendering.
 */
module;

export module helios.rendering.model.config.PrimitiveType;


export namespace helios::rendering::model::config {

    /**
     * @brief Defines how vertices are assembled into primitives during rendering.
     *
     * This enum provides API-agnostic representations for common primitive
     * assembly modes (points, lines, triangles, and their strip/fan variants).
     * Backends should map these values to the corresponding native topology
     * constants.
     *
     * Example (pseudo-code mapping to OpenGL):
     * @code{.cpp}
     * switch (primitiveType) {
     *   case PrimitiveType::Points:        return GL_POINTS;
     *   case PrimitiveType::Lines:         return GL_LINES;
     *   case PrimitiveType::LineLoop:      return GL_LINE_LOOP;
     *   case PrimitiveType::LineStrip:     return GL_LINE_STRIP;
     *   case PrimitiveType::Triangles:     return GL_TRIANGLES;
     *   case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
     *   case PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
     *   default: // unreachable                 return GL_TRIANGLES;
     * }
     * @endcode
     *
     * @see MeshConfig (default is `PrimitiveType::Triangles`)
     */
    enum class PrimitiveType {
        /**
         * Draw each vertex as a single point.
         */
        Points,

        /**
         * Treat each pair of vertices as an independent line segment.
         */
        Lines,

        /**
         * Connect vertices in sequence and close the loop (last -> first).
         */
        LineLoop,

        /**
         * Connect vertices in sequence forming a continuous polyline.
         */
        LineStrip,

        /**
         * Interpret each group of three vertices as an independent triangle.
         */
        Triangles,

        /**
         * Build a connected strip of triangles sharing vertices.
         */
        TriangleStrip,

        /**
         * Build triangles sharing the first vertex (fan topology).
         */
        TriangleFan,

        /**
         * @brief Sentinel value equal to the number of primitive types.
         */
        size_
    };

}