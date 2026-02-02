/**
 * @file Ellipse.ixx
 * @brief Basic ellipse shape for 2D rendering.
 */
module;

#include <vector>
#include <cmath>
#include <numbers>

export module helios.rendering.asset.shape.basic.Ellipse;

import helios.rendering.asset.shape.Shape;
import helios.rendering.Vertex;
import helios.math.types;
import helios.rendering.mesh.PrimitiveType;

using namespace helios::rendering;

export namespace helios::rendering::asset::shape::basic {

    /**
     * @brief Basic Ellipse Shape (2D, in XY plane).
     *
     * Generates a 2D ellipse mesh, centered at the origin on the XY plane (Z=0).
     * An ellipse is defined by two radii: one along the X-axis (semi-major or semi-minor)
     * and one along the Y-axis. When both radii are equal, the ellipse degenerates to a circle.
     *
     * The normals point in the positive Z direction.
     * The indices are arranged as a Triangle Fan (for GL_TRIANGLES).
     */
    struct Ellipse : public Shape {

    private:
        /**
         * @brief Dynamically creates the vertex list for an ellipse.
         *
         * Generates one center vertex (Index 0) and 'segments' vertices on the circumference.
         *
         * @param radiusX The semi-axis length along the X-axis.
         * @param radiusY The semi-axis length along the Y-axis.
         * @param segments The number of segments used to approximate the ellipse curve.
         * @return A vector of vertices defining the ellipse mesh.
         */
        static std::vector<Vertex> generateVertices(float radiusX, float radiusY, unsigned int segments) {
            std::vector<Vertex> vertices;
            // +1 for the center vertex
            vertices.reserve(segments + 1);

            // center vertex (index 0)
            vertices.push_back({
                {0.0f, 0.0f, 0.0f}, // Position
                {0.0f, 0.0f, 1.0f}, // Normal
                {0.5f, 0.5f}        // TexCoord (center of texture)
            });

            // circumference vertices (Indices 1 to segments)
            for (unsigned int i = 0; i < segments; ++i) {
                // calculate the angle for this segment
                const float angle = (static_cast<float>(i) / static_cast<float>(segments)) * 2.0f * std::numbers::pi_v<float>;

                float x = radiusX * std::cos(angle);
                float y = radiusY * std::sin(angle);

                // texture coordinates: map to [0, 1] range
                // use the larger radius to normalize for consistent UV mapping
                float maxRadius = std::max(radiusX, radiusY);
                float u = (x / maxRadius) * 0.5f + 0.5f;
                float v = (y / maxRadius) * 0.5f + 0.5f;

                vertices.push_back({
                    {x, y, 0.0f},
                    {0.0f, 0.0f, 1.0f},
                    {u, v}
                });
            }
            return vertices;
        }

        /**
         * @brief Creates the index list for a Triangle Fan (for GL_TRIANGLES).
         *
         * Each triangle consists of (center, point_i, point_i+1).
         *
         * @param segments The number of segments used to build the ellipse.
         * @return A vector of indices defining the triangle fan.
         */
        static std::vector<unsigned int> generateIndices(unsigned int segments) {
            std::vector<unsigned int> indices;
            indices.reserve(segments * 3); // 'segments' triangles, 3 indices per triangle

            for (unsigned int i = 1; i <= segments; ++i) {
                indices.push_back(0); // center vertex
                indices.push_back(i); // current circumference vertex

                // next circumference vertex
                // for the last triangle (i == segments), connect back to Vertex 1
                unsigned int next_i = (i == segments) ? 1 : (i + 1);
                indices.push_back(next_i);
            }
            return indices;
        }

    public:
        /**
         * @brief Creates a default ellipse (radiusX = 1.0, radiusY = 0.5, 64 segments).
         */
        Ellipse() : Ellipse(1.0f, 0.5f, 64) {}

        /**
         * @brief Creates an ellipse with specific radii and segment count.
         *
         * @param radiusX The semi-axis length along the X-axis.
         * @param radiusY The semi-axis length along the Y-axis.
         * @param segments The number of triangle segments that make up the ellipse.
         *                 More segments result in a smoother curve.
         */
        explicit Ellipse(float radiusX, float radiusY, unsigned int segments) :
            Shape(
                generateVertices(radiusX, radiusY, segments),
                generateIndices(segments)
            ) {}

        /**
         * @brief Returns the primitive type used for rendering.
         * @return PrimitiveType::Triangles for triangle-based rendering.
         */
        [[nodiscard]] helios::rendering::mesh::PrimitiveType primitiveType() const noexcept override {
            return mesh::PrimitiveType::Triangles;
        }
    };

}

