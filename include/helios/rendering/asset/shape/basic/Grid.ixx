/**
 * @file Grid.ixx
 * @brief Basic grid shape for rendering.
 */
module;


#include <stdexcept>
#include <vector>

export module helios.rendering.asset.shape.basic.Grid;

import helios.rendering.asset.shape.Shape;
import helios.rendering.Vertex;
import helios.math.types;
import helios.rendering.mesh.PrimitiveType;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief A configurable grid shape for rendering reference lines in 3D space.
     *
     * @details The Grid class generates a planar grid mesh composed of line segments. The grid
     * is centered at the origin and spans from -1 to +1 on both the X and Y axes. The number
     * of subdivisions along each axis can be configured via constructor parameters.
     *
     * The generated grid lies in the XY-plane with all vertices having a Z-coordinate of 0.
     * All normals point in the positive Z direction (0, 0, 1).
     *
     * Typical use cases include:
     * - Scene orientation and scale reference
     * - Debug visualization
     * - Editor overlays
     *
     * Example usage:
     * ```cpp
     * // Create a 10x10 grid
     * auto grid = std::make_shared<Grid>(10, 10);
     * scene->addRenderable(grid);
     * ```
     *
     * @note The grid uses `PrimitiveType::Lines` for rendering, not triangles.
     *
     * @see Shape
     */
    struct Grid : public Shape {

    private:
        /**
         * @brief Generates vertex and index data for the grid geometry.
         *
         * @details Creates a grid of vertices in the XY-plane spanning from (-1, -1) to (1, 1).
         * Vertices are generated row by row, and indices are created to form horizontal and
         * vertical line segments connecting adjacent vertices.
         *
         * @param segmentsX Number of subdivisions along the X-axis.
         * @param segmentsY Number of subdivisions along the Y-axis.
         *
         * @return A pair containing the vertex list and the index list for line rendering.
         *
         * @throws std::invalid_argument If either segmentsX or segmentsY is less than 2.
         */
        static std::pair<
            std::vector<Vertex>,
            std::vector<unsigned int>
        > generate(
            unsigned int segmentsX, unsigned int segmentsY
        ) {
            if (segmentsY < 2 || segmentsX < 2) {
                throw std::invalid_argument("Segments must at least be 2.");
            }
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            vertices.reserve((segmentsX + 1) * (segmentsY + 1));

            const float quotX = 2.0f/static_cast<float>(segmentsX);
            const float quotY = 2.0f/static_cast<float>(segmentsY);

            for (int i = 0; i < segmentsX+1; i++) {
                float x = -1.0f + i * quotX;

                for (int j = 0; j < segmentsY + 1; j++) {
                    float y = -1.0f + j * quotY;

                    vertices.push_back({
                        {x, y, 0.0f},
                        {0.0f, 0.0f, 1.0f},
                        {0.5f, 0.5f}
                    });

                    if (i >= segmentsX || j >= segmentsY) {
                        continue;
                    }

                    if (i > 0) {
                        unsigned int idx = (i) * (segmentsY + 1) + j;
                        indices.push_back(idx);
                        indices.push_back(idx +1);
                    }

                    if (j > 0) {
                        unsigned int idx = i * (segmentsY + 1) + j;
                        indices.push_back(idx);
                        indices.push_back(idx + 1 + segmentsY);
                    }
                }

            }

            return std::pair(vertices, indices);
        }

        /**
         * @brief Private constructor that takes pre-generated geometry data.
         *
         * @details Used internally by the public constructors to initialize the base Shape
         * class with vertex and index data.
         *
         * @param data A pair containing vertex data and index data for the grid.
         */
        explicit Grid(std::pair<std::vector<Vertex>, std::vector<unsigned int>> data) :
             Shape(std::move(data.first), std::move(data.second))
        {
        }


    public:
        /**
         * @brief Creates a default grid with 2 segments per coordinate axis.
         *
         * @details Constructs a simple 2x2 grid, resulting in a cross-like pattern
         * with one horizontal and one vertical subdivision.
         */
        Grid() : Grid(2, 2) {}

        /**
         * @brief Creates a grid with a specified number of subdivisions.
         *
         * @details Constructs a grid with the given number of segments along each axis.
         * Higher segment counts produce finer grids with more lines.
         *
         * @param segmentsX Number of subdivisions along the X-axis. Must be at least 2.
         * @param segmentsY Number of subdivisions along the Y-axis. Must be at least 2.
         *
         * @throws std::invalid_argument If either segmentsX or segmentsY is less than 2.
         */
        explicit Grid(unsigned int segmentsX, unsigned int segmentsY) :
            Grid(generate(segmentsX, segmentsY))
        { }

        /**
         * @brief Returns the primitive type used for rendering this shape.
         *
         * @return `PrimitiveType::Lines` since the grid is rendered as line segments.
         */
        [[nodiscard]] helios::rendering::mesh::PrimitiveType primitiveType() const noexcept override {
            return mesh::PrimitiveType::Lines;
        }
    };

}