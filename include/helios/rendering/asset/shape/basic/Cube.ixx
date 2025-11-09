module;


#include <vector>

export module helios.rendering.asset.shape.basic.Cube;

import helios.rendering.asset.shape.Shape;

import helios.rendering.Vertex;
import helios.rendering.model.config.PrimitiveType;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief Basic Cube Shape.
     */
    struct Cube : public Shape {

        Cube():
        Shape(
            std::vector<Vertex>{
                // n (z = 0.5)
                {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}}, // Bottom-left
                {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}}, // Bottom-right
                {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}}, // Top-right
                {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}}, // Top-left

                // f (z = -0.5)
                {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom-left
                {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-right
                {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}}, // Top-right
                {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}}, // Top-left

                // l (x = -0.5)
                {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
                {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
                {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right
                {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // Top-left

                // r (x = 0.5)
                {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-left
                {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-right
                {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}, // Top-right
                {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}}, // Top-left

                // b (y = -0.5)
                {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}}, // Bottom-left
                {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}}, // Bottom-right
                {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}}, // Top-right
                {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}}, // Top-left

                // t (y = 0.5)
                {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // Bottom-left
                {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // Bottom-right
                {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}}, // Top-right
                {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}  // Top-left
            },
            std::vector<unsigned int>{
                // n
                0, 1, 2,    2, 3, 0,
                // f
                4, 5, 6,    6, 7, 4,
                // l
                8, 9, 10,   10, 11, 8,
                // r
                12, 13, 14, 14, 15, 12,
                // b
                16, 17, 18, 18, 19, 16,
                // t
                20, 21, 22, 22, 23, 20
            }
        ) {}

        [[nodiscard]] helios::rendering::model::config::PrimitiveType primitiveType() const noexcept override {
            return model::config::PrimitiveType::LineLoop;
        }
    };

}