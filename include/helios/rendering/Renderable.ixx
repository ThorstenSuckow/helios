module;

#include <memory>
#include <optional>
#include <stdexcept>

export module helios.rendering.Renderable;

import helios.rendering.model.Mesh;
import helios.rendering.model.Material;

import helios.rendering.RenderableDrawCallConfig;

import helios.math.types;

import helios.util.log.LogManager;
import helios.util.log.Logger;

#define HELIOS_LOG_SCOPE "helios::rendering::Renderable"
export namespace helios::rendering {

    /**
     * Representative of a configurable Renderable rendered by the underlying GL API.
     *
     * A Renderable is an aggregate consisting of Material and a Mesh, providing
     * geometric primitives and material information.
     *
     * @todo parent-child relationship
     */
    class Renderable {

    protected:
        Renderable() = default;

        /**
         * A unique pointer to the Mesh owned by this Renderable.
         */
        std::unique_ptr<helios::rendering::model::Mesh> mesh_ = nullptr;

        /**
         * A unique pointer to the Material owned by the Renderable.
         */
        std::unique_ptr<helios::rendering::model::Material> material_ = nullptr;

        /**
         * The logger used with this Renderable instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:

        virtual ~Renderable() = default;

        /**
         * Creates a new Renderable instance.
         *
         * @param mesh A unique pointer to the Mesh owned by this Renderable.
         * @param material A unique pointer to the Material owned by this Renderable.
         *
         * @throws std::invalid_argument if either mesh or material are null.
         */
        explicit Renderable(
            std::unique_ptr<helios::rendering::model::Mesh> mesh,
            std::unique_ptr<helios::rendering::model::Material> material);


        /**
         * Returns a const reference to the Mesh owned by this Renderable.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return Mesh
         */
        [[nodiscard]] const helios::rendering::model::Mesh& mesh() const noexcept;


        /**
         * Returns a const reference to the Material owned by this Renderable.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return Material
         */
        [[nodiscard]] const helios::rendering::model::Material& material() const noexcept;


    };

}