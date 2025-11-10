/**
 * @brief Primitive assembly types used by rendering backends
 */
module;

export module helios.rendering.model.config.PrimitiveType;


export namespace helios::rendering::model::config {

    /**
     * @brief Defines how vertices are assembled into primitives during rendering.
     *
     * This enum provides API-agnostic representations for various ways to
     * interpret vertex data into geometric primitives.
     *
     * @see https://wikis.khronos.org/opengl/Primitive
     */
    enum class PrimitiveType {
        Points,
        Lines,
        LineLoop,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan
    };

}