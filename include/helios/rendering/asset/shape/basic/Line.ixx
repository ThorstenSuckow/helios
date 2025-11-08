module;


#include <vector>

export module helios.rendering.asset.shape.basic.Line;

import helios.rendering.asset.shape.Shape;

import helios.rendering.Vertex;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief Basic Line Shape.
     *
     * Creates vertex and index data for a single line segment. By default, this shape
     * defines a line spanning from the origin (0, 0 ,0) to the point (1, 1, 0).
     *
     * @note the indices (0,1) suggest rendering with PrimitiveType::Lines.
     */
    struct Line : public Shape {

        Line(): Shape(
            std::vector<Vertex>{

                {{ 0.0f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 1.0f,  1.f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
            },
            std::vector<unsigned int>{
                0, 1
            }
        ) {}

    };

}