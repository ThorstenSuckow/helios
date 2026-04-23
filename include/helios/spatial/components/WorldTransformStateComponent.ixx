/**
 * @file WorldTransformStateComponent.ixx
 * @brief Component for managing world transformations of an Entity.
 */
module;

export module helios.spatial.components.WorldTransformStateComponent;

import helios.math.types;

import helios.core.spatial;

export namespace helios::spatial::components {


    template<typename THandle>
    class WorldTransformStateComponent {

        /**
         * @brief The computed world transformation matrix.
         */
        helios::math::mat4f worldTransform_ = helios::math::mat4f::identity();

       
    public:

        explicit WorldTransformStateComponent(const helios::math::mat4f& m) noexcept
        : worldTransform_(m) {}

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        WorldTransformStateComponent(const WorldTransformStateComponent& other) :
            worldTransform_(other.worldTransform_) {}

        WorldTransformStateComponent& operator=(const WorldTransformStateComponent&) = default;
        WorldTransformStateComponent(WorldTransformStateComponent&&) noexcept = default;
        WorldTransformStateComponent& operator=(WorldTransformStateComponent&&) noexcept = default;


        /**
         * @brief Sets the world transform matrix directly.
         *
         * @param m The new world transform matrix.
         */
        void setWorldTransform(const helios::math::mat4f& m) noexcept {
            worldTransform_ = m;
        }

        /**
         * @brief Returns the current world transform matrix.
         *
         * @return Const reference to the world transform.
         */
        const helios::math::mat4f& worldTransform() const noexcept {
            return worldTransform_;
        }

    };

}

