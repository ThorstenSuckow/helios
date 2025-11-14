/**
 * @file Line.ixx
 * @brief Basic line shape for rendering.
 */
module;


#include <vector>

export module helios.rendering.asset.shape.basic.Line;

import helios.rendering.asset.shape.Shape;
import helios.rendering.Vertex;
import helios.rendering.model.config.PrimitiveType;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief Basic Line Shape.
     *
     * Creates vertex and index data for a single line segment. By default, this shape
     * defines a line spanning from the origin (-1, -1, 0) to the point (1, 1, 0).
     */
    struct Line : public Shape {

        Line(): Shape(
            std::vector<Vertex>{

                {{ -1.0f,  -1.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 1.0f,  1.f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
            },
            std::vector<unsigned int>{
                0, 1
            }
        ) {}

        [[nodiscard]] helios::rendering::model::config::PrimitiveType primitiveType() const noexcept override {
            return model::config::PrimitiveType::Lines;
        }

    };

}