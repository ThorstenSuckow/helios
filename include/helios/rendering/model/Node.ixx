module;

#include <memory>
#include <stdexcept>

export module helios.rendering.model.Node;

import helios.rendering.model.Mesh;
import helios.rendering.model.Material;

import helios.util.log.LogManager;
import helios.util.log.Logger;

#define HELIOS_LOG_SCOPE "helios::rendering::model::Node"
export namespace helios::rendering::model {

    /**
     * Representative of a configurable node rendered by the underlying GL API.
     *
     * A node is an aggregate consisting of Material and a Mesh, providing
     * geometric primitives and material information.
     *
     * @todo parent-child relationship
     */
    class Node {

    protected:
        Node() = default;

        /**
         * A unique pointer to the Mesh owned by this Node.
         */
        std::unique_ptr<Mesh> mesh_ = nullptr;

        /**
         * A unique pointer to the Material owned by the Node.
         */
        std::unique_ptr<Material> material_ = nullptr;

        /**
         * The logger used with this Node instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:

        virtual ~Node() = default;

        /**
         * Creates a new Node instance.
         *
         * @param mesh A unique pointer to the Mesh owned by this Node.
         * @param material A unique pointer to the Material owned by this node.
         *
         * @throws std::invalid_argument if either mesh or material are null.
         */
        explicit Node(
            std::unique_ptr<Mesh> mesh,
            std::unique_ptr<Material> material);


        /**
         * Delegate to the draw calls of the underlying GL API.
         */
        virtual void draw() const noexcept = 0;


        /**
         * Returns a const reference to the Mesh owned by this Node.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return Mesh
         */
        [[nodiscard]] const Mesh& mesh() const noexcept;


        /**
         * Returns a const reference to the Material owned by this Node.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return Material
         */
        [[nodiscard]] const Material& material() const noexcept;

    };

}