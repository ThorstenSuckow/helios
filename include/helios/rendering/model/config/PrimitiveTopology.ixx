module;

export module helios.rendering.model.config.PrimitiveTopology;


export namespace helios::rendering::model::config {

    /**
     * @brief Defines how vertices are assembled into primitives during rendering.
     *
     * This enum provides API-agnostic representations for various ways to
     * interpret vertex data into geometric primitives.
     */
    enum class PrimitiveTopology {
        Points,
        Lines,
        LineLoop,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan
    };

}