module;


export module helios.scene.components.LookAtComponent;


import helios.math;

using namespace helios::math;
export namespace helios::scene::components {

    template<typename THandle>
    class LookAtComponent {

        vec3f target_{};
        vec3f up_{};

    public:

        LookAtComponent() = default;

        void lookAt(const vec3f target, const vec3f up) noexcept {
            target_ = target;
            up_ = up;
        }

        [[nodiscard]] vec3f target() const noexcept {
            return target_;
        }

        [[nodiscard]] vec3f up() const noexcept {
            return up_;
        }

    };

}