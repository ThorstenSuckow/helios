module;

#include <vector>

export module helios.rendering.core:MeshData;

import :Vertex;

export namespace helios::rendering {

    struct MeshData {
        std::vector<Vertex> vertices;
    };

}
