module;

#include <memory>
#include <vector>

export module helios.rendering.asset.shape.Shape;

import helios.rendering.core.Vertex;

using namespace helios::rendering::core;

export namespace helios::rendering::asset::shape {

    struct Shape{

        std::shared_ptr<const std::vector<Vertex>> vertices = {};
        std::shared_ptr<const std::vector<unsigned int>> indices = {};

        Shape(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
            vertices(std::make_shared<const std::vector<Vertex>>(vertices)),
            indices(std::make_shared<const std::vector<unsigned int>>(indices))
        {}

        virtual ~Shape() = default;
    };

}