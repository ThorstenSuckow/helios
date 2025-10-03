module;

#include <memory>
#include <vector>

export module helios.rendering.asset.shape.Shape;

import helios.rendering.Vertex;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape {

    /**
     * Struct representing geometric primitives and indices for various shapes.
     * provides immutable, shared access to vertices and indices of the
     * represented geometric shape.
     */
    struct Shape {

        virtual ~Shape() = default;

        /**
         * Shared pointer to an immutable vector of vertices for this shape.
         */
        std::shared_ptr<const std::vector<Vertex>> vertices = {};

        /**
         * Shared pointer to an immutable vector of indices for this shape.
         */
        std::shared_ptr<const std::vector<unsigned int>> indices = {};

        /**
         * Creates a new shape from the given vertices and indices.
         * The data is copied into immutable shared vectors.
         *
         * @param vertices A vector of vertices representing the shape's geometry
         * @param indices A vector of unsigned integers representing the shape's indices.
         */
        Shape(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
            vertices(std::make_shared<const std::vector<Vertex>>(std::move(vertices))),
            indices(std::make_shared<const std::vector<unsigned int>>(std::move(indices)))
        {}
    };

}