module;

export module helios.rendering.core:Vertex;

import helios.math.types;

export namespace helios::rendering::core {

    class Vertex {
    private:
        const math::vec3 position_;

    public:
        explicit Vertex(const math::vec3 position) : position_(position) {}

        [[nodiscard]] const math::vec3& position() const {
            return position_;
        }
    };


}
