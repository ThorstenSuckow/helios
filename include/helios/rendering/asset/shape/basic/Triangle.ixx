/**
 * @file Triangle.ixx
 * @brief Basic Triangle (line) shape for rendering.
 */
module;


#include <vector>

export module helios.rendering.asset.shape.basic.Triangle;

import helios.rendering.asset.shape.Shape;
import helios.rendering.Vertex;
import helios.rendering.mesh.PrimitiveType;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief Basic Triangle Shape.
     *
     * Creates vertex and index data for a single Triangle line segment. By default, this shape
     * defines three vertices  (-1, -1, 0), (-1, 1, 0) to the point (1, 0, 0).
     */
    struct Triangle : public Shape {

        Triangle(): Shape(
            std::vector<Vertex>{

                {{ -0.5f,  -0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ -0.5f,  0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.5f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}
            },
            std::vector<unsigned int>{
                0, 1, 2, 0
            }
        ) {}

        [[nodiscard]] helios::rendering::mesh::PrimitiveType primitiveType() const noexcept override {
            return mesh::PrimitiveType::Lines;
        }

    };

}