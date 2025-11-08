module;

#include <memory>
#include <optional>
#include <stdexcept>

export module helios.rendering.Renderable;

import helios.rendering.model.Mesh;
import helios.rendering.model.MaterialInstance;


import helios.math.types;

import helios.util.log.LogManager;
import helios.util.log.Logger;

#define HELIOS_LOG_SCOPE "helios::rendering::Renderable"
export namespace helios::rendering {

    /**
     * @brief Representative of a configurable Renderable rendered by the underlying GL API.
     *
     * A Renderable is an aggregate consisting of Material and a Mesh, providing
     * geometric primitives and material information.
     *
     * @todo a Renderable should be configured with optional, selected overrides for the
     * associated mesh/material, so that for the used shared mesh/material objects, individual
     * draw configs are possible.
     */
    class Renderable {

    protected:
        Renderable() = default;

        /**
         * @brief A shared pointer to the Mesh this Renderable uses.
         */
        std::shared_ptr<helios::rendering::model::Mesh> mesh_ = nullptr;

        /**
         * @brief A shared pointer to the MaterialInstance owned by the Renderable.
         */
        std::shared_ptr<helios::rendering::model::MaterialInstance> materialInstance_ = nullptr;

        /**
         * @brief The logger used with this Renderable instance.
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
         * @brief Delete copy constructor.
         */
        Renderable(const Renderable&) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        Renderable& operator=(const Renderable&)= delete;

        /**
         * @brief Delete move constructor.
         */
        Renderable(Renderable&&) noexcept = delete;

        /**
         * @brief Delete move assignment operator.
         */
        Renderable& operator=(Renderable&&) noexcept = delete;

        /**
         * @brief Creates a new Renderable instance.
         *
         * @param mesh A shared pointer to the Mesh associated with this Renderable.
         * @param materialInstance A shared pointer to the MaterialInstance owned by this Renderable.
         *
         * @throws std::invalid_argument if either mesh or material are null.
         */
        explicit Renderable(
            std::shared_ptr<helios::rendering::model::Mesh> mesh,
            std::shared_ptr<helios::rendering::model::MaterialInstance> materialInstance);


        /**
         * @brief Returns a shared ptr to the Mesh this Renderable uses.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return Mesh
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::model::Mesh> mesh() const noexcept;


        /**
         * @brief Returns a const reference to the Material shared with this Renderable.
         * The returned data is guaranteed to be a valid reference to existing data.
         *
         * @return MaterialInstance
         */
        [[nodiscard]] const helios::rendering::model::MaterialInstance& materialInstance() const noexcept;


    };

}