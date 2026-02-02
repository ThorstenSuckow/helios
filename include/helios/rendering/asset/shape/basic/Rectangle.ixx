/**
 * @file Rectangle.ixx
 * @brief Basic rectangle shape for rendering.
 */
module;

#include <vector>

export module helios.rendering.asset.shape.basic.Rectangle;

import helios.rendering.asset.shape.Shape;

import helios.rendering.Vertex;
import helios.rendering.mesh.PrimitiveType;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief Basic Rectangle Shape.
     *
     * Creates vertex and index data for a single 2D rectangle.
     *
     * @note Uses indices {0, 1, 2, 3, 0} with PrimitiveType::LineStrip to render the rectangle outline.
     */
    struct Rectangle : public Shape {

        Rectangle(): Shape(
            std::vector<Vertex>{
                {{ -0.5f,  -0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ -0.5f,  0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.5f,  0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.5f,  -0.5f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
            },
            std::vector<unsigned int>{
                0, 1, 2, 3, 0
            }
        ) {}

        [[nodiscard]] helios::rendering::mesh::PrimitiveType primitiveType() const noexcept override {
            return mesh::PrimitiveType::LineStrip;
        }

    };

}