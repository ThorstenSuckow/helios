/**
 * @file Mesh.ixx
 * @brief Mesh abstraction: immutable vertex/index data and configuration.
 */
module;

#include <memory>
#include <stdexcept>
#include <vector>


export module helios.rendering.model.Mesh;

import helios.math.types;
import helios.rendering.model.config.MeshConfig;
import helios.rendering.Vertex;
import helios.rendering.asset.shape.Shape;
import helios.util.log.LogManager;
import helios.util.log.Logger;
import helios.math.types;


#define HELIOS_LOG_SCOPE "helios::rendering::model::Mesh"
export namespace helios::rendering::model {

    /**
     * @brief Representative of a renderable 3D mesh.
     *
     * A Mesh instance contains vertex data and indices provided by geometric shapes.
     * Meshes contain references to immutable, shared vertex data and indices.
     * Concrete implementations will handle resource management.
     *
     * @see [Gre19, 631]
     */
    class Mesh {

    protected:

        Mesh() = default;


        /**
         * @brief Flag indicating whether the mesh requires updates to its computed data.
         *
         * This variable is used to track whether the mesh's derived properties, such
         * as its axis-aligned bounding box (AABB), are outdated and need to be recalculated.
         * It is marked as mutable to allow modification within const member functions.
         */
        mutable bool needsUpdate_ = true;


        /**
         * @brief Represents the axis-aligned bounding box (AABB) for the mesh.
         *
         * This member variable stores the computed or cached AABB for the mesh in
         * 3D space. The AABB is a minimal bounding box aligned with the coordinate
         * axes that encloses all the vertices of the mesh. It is marked mutable to
         * allow updates during read-only operations, such as recalculating bounds
         * as vertex positions are modified or updated dynamically.
         *
         * @see helios::math::aabb
         */
        mutable helios::math::aabbf aabb_{};

        /**
         * @brief Shared pointer to the raw, immutable vertices.
         */
        std::shared_ptr<const std::vector<Vertex>> vertices_ = nullptr;

        /**
         * @brief Shared pointer to the raw, immutable indices.
         */
        std::shared_ptr<const std::vector<unsigned int>> indices_ = nullptr;

        /**
         * @brief Shared pointer to the MeshConfig used with this Mesh.
         */
        std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig_;

        /**
         * @todo
         * std::shared_ptr<const std::vector<Texture>> textures_;
         */

        /**
         * @brief Initializes the Mesh by setting up vertex attributes and buffers with the
         * underlying GL API.
         * This method should be called _once_ by the derived class before using
         * this Mesh in a rendering pass.
         *
         * @throws std::runtime_error if the Mesh could not be initialized.
         */
        virtual void init() = 0;

        /**
         * @brief Shared logger instance for all Mesh objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:

        virtual ~Mesh() = default;

        /**
         * @brief Creates a new Mesh instance.
         *
         * @param vertices A shared_ptr to a vector of Vertex.
         * @param indices A shared_ptr to a vector of indices.
         * @param meshConfig A shared_ptr to the MeshConfig used with this Mesh.
         *
         * @throws std::invalid_argument if either "vertices", "indices" or meshConfig is a null shared_ptr
         */
        explicit Mesh(
            std::shared_ptr<const std::vector<Vertex>> vertices,
            std::shared_ptr<const std::vector<unsigned int>> indices,
            std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
        );

        /**
         * @brief Creates a new Mesh instance from the specified Shape.
         *
         * @param shape A const reference to the Shape.
         * @param meshConfig A shared_ptr to the MeshConfig used with this Mesh.
         *
         * @throws std::invalid_argument if meshConfig is a null shared_ptr, or if the
         * shape contained null data.
         */
        explicit Mesh(
            const helios::rendering::asset::shape::Shape& shape,
            std::shared_ptr<const helios::rendering::model::config::MeshConfig> meshConfig
        );

        /**
         * @brief Returns the index count of this Mesh.
         *
         * @return The number of indices handled by this Mesh.
         */
        [[nodiscard]] const unsigned int indexCount() const noexcept;

        /**
         * @brief Returns a const reference to the underlying vertices.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return A const ref to the vector of Vertex.
         */
        [[nodiscard]] const std::vector<Vertex>& vertices() const noexcept;

        /**
         * @brief Returns a const reference to the underlying indices.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return A const ref to the vector of indices.
         */
        [[nodiscard]] const std::vector<unsigned int>& indices() const noexcept;

        /**
         * @brief Returns a const reference to MeshConfig used with this Mesh.
         * The returned MeshConfig is guaranteed to be a valid reference to existing data.
         *
         * @return The MeshConfig used with this Mesh.
         */
        [[nodiscard]] const helios::rendering::model::config::MeshConfig& meshConfig() const noexcept;


        /**
         * @brief Retrieves the axis-aligned bounding box (AABB) of the mesh.
         *
         * The method calculates and returns the AABB of the mesh, which encloses
         * the mesh's vertices. If the mesh requires an update to its bounding box,
         * this method recomputes the bounds based on the vertex positions and caches
         * the result for future calls. The returned AABB represents the minimal
         * rectangle in 3D space that contains all the vertices of the mesh.
         *
         * @return Reference to the computed axis-aligned bounding box.
         */
        [[nodiscard]] const helios::math::aabbf& aabb() const noexcept;
    };

}