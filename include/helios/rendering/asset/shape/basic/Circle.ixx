module;

#include <vector>
#include <cmath>
#include <numbers>

export module helios.rendering.asset.shape.basic.Circle;

import helios.rendering.asset.shape.Shape;
import helios.rendering.Vertex;
import helios.math.types;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief Basic Circle Shape (2D, in XY plane).
     *
     * Generates a 2D circle mesh, centered at the origin on the XY plane (Z=0).
     * The normals point in the positive Z direction.
     * The indices are arranged as a Triangle Fan (for GL_TRIANGLES).
     */
    struct Circle : public Shape {

    private:
        /**
         * @brief Dynamically creates the vertex list for a circle.
         * Generates one center vertex (Index 0) and 'segments' vertices on the circumference.
         */
        static std::vector<Vertex> generateVertices(float radius, unsigned int segments) {
            std::vector<Vertex> vertices;
            // +1 for the center vertex
            vertices.reserve(segments + 1);

            // center vertex (index 0)
            vertices.push_back({
                {0.0f, 0.0f, 0.0f}, // Position
                {0.0f, 0.0f, 1.0f}, // Normal
                {0.5f, 0.5f}        // TexCoord
            });

            //  circumference vertices (Indices 1 to segments)
            for (unsigned int i = 0; i < segments; ++i) {
                // calculate the angle for this segment
                float angle = (static_cast<float>(i) / segments) * 2.0f * std::numbers::pi_v<float>;

                float x = radius * std::cos(angle);
                float y = radius * std::sin(angle);

                // texture coordinates: map [-radius, +radius] to [0, 1]
                float u = (x / radius) * 0.5f + 0.5f;
                float v = (y / radius) * 0.5f + 0.5f;

                vertices.push_back({
                    {x, y, 0.0f},
                    {0.0f, 0.0f, 1.0f},
                    {u, v}
                });
            }
            return vertices;
        }

        /**
         * @brief Creates the index list for a `Triangle Fan` (for GL_TRIANGLES).
         * Each triangle consists of (center, point_i, point_i+1).
         */
        static std::vector<unsigned int> generateIndices(unsigned int segments) {
            std::vector<unsigned int> indices;
            indices.reserve(segments * 3); // 'segments' triangles, 3 indices per triangle

            for (unsigned int i = 1; i <= segments; ++i) {
                indices.push_back(0); // center vertex
                indices.push_back(i); // current circumference vertex

                // next circumference vertex
                // for the last triangle (i == segments),
                // connect back to Vertex 1
                unsigned int next_i = (i == segments) ? 1 : (i + 1);
                indices.push_back(next_i);
            }
            return indices;
        }

    public:
        /**
         * @brief Creates a default circle (radius 0.5, 64 segments).
         */
        Circle() : Circle(0.5f, 64) {}

        /**
         * @brief Creates a circle with a specific radius and segment count.
         *
         * @param radius The radius of the circle (from center to circumference).
         * @param segments The number of triangle segments that make up the circle.
         * (More segments = smoother circle).
         */
        explicit Circle(float radius, unsigned int segments) :
            Shape(
                generateVertices(radius, segments),
                generateIndices(segments)
            ) {}
    };

}