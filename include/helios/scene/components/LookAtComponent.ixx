/**
 * @file LookAtComponent.ixx
 * @brief Stores look-at target data for scene-related entities.
 */
module;


export module helios.scene.components.LookAtComponent;


import helios.math;

using namespace helios::math;
export namespace helios::scene::components {

    /**
     * @brief Component that stores a target position and up vector.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    class LookAtComponent {

        vec3f target_{};
        vec3f up_{};

    public:

        /** @brief Default-constructs an empty look-at state. */
        LookAtComponent() = default;

        /**
         * @brief Updates target and up vectors.
         *
         * @param target Target position.
         * @param up Up direction.
         */
        void lookAt(const vec3f target, const vec3f up) noexcept {
            target_ = target;
            up_ = up;
        }

        /** @brief Returns the current target position. */
        [[nodiscard]] vec3f target() const noexcept {
            return target_;
        }

        /** @brief Returns the current up direction. */
        [[nodiscard]] vec3f up() const noexcept {
            return up_;
        }

    };

}