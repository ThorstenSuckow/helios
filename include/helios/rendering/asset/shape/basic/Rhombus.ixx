/**
 * @file Rhombus.ixx
 * @brief Diamond/rhombus shape primitive for rendering.
 */
module;

#include <vector>

export module helios.rendering.asset.shape.basic.Rhombus;

import helios.rendering.asset.shape.Shape;
import helios.rendering.Vertex;
import helios.rendering.mesh.PrimitiveType;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief A 2D rhombus (diamond) shape primitive.
     *
     * Creates a diamond shape centered at the origin with vertices at:
     * - Left:   (-0.5, 0.0)
     * - Top:    (0.0, 0.25)
     * - Right:  (0.5, 0.0)
     * - Bottom: (0.0, -0.25)
     *
     * The shape spans 1.0 units horizontally and 0.5 units vertically.
     *
     * ```
     *        (0, 0.25)
     *           /\
     *          /  \
     *         /    \
     * (-0.5,0)      (0.5,0)
     *         \    /
     *          \  /
     *           \/
     *        (0, -0.25)
     * ```
     *
     * Default primitive type is Lines (wireframe rendering).
     */
    struct Rhombus : public Shape {

        /**
         * @brief Constructs a rhombus shape with predefined vertices.
         */
        Rhombus(): Shape(
            std::vector<Vertex>{

                {{ -0.5f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.0f,  0.25f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.0f,  -0.25f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
                {{ 0.5f,  0.0f,  0.0f}, { 0.0f,  0.0f,  0.0f}, {0.0f, 0.0f}}

            },
            std::vector<unsigned int>{
               2, 3, 1, 0, 2, 1,
            }
        ) {}

        /**
         * @brief Returns the primitive type for rendering.
         *
         * @return PrimitiveType::Lines for wireframe rendering.
         */
        [[nodiscard]] helios::rendering::mesh::PrimitiveType primitiveType() const noexcept override {
            return mesh::PrimitiveType::Lines;
        }

    };

}