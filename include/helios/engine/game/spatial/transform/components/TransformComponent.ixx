/**
 * @file TransformComponent.ixx
 * @brief Component for managing local and world transformations of a GameObject.
 */
module;

export module helios.engine.game.spatial.transform.components.TransformComponent;

import helios.math.types;
import helios.engine.ecs.CloneableComponent;
import helios.core.spatial.Transform;

export namespace helios::engine::game::spatial::transform::components {

    /**
     * @brief Component that holds transformation data (position, rotation, scale).
     *
     * @details The TransformComponent manages the spatial state of a GameObject.
     * It stores the local transform (relative to parent) and the computed world transform.
     * Changes to local properties mark the component as dirty, signaling systems to
     * recompute the world transform.
     */
    class TransformComponent : public helios::engine::ecs::CloneableComponent<TransformComponent> {

        /**
         * @brief The local transformation (translation, rotation, scale).
         */
        helios::core::spatial::Transform transform_{};

        /**
         * @brief Flag indicating if the transform has changed since the last update.
         */
        bool isDirty_= false;

        /**
         * @brief The computed world transformation matrix.
         */
        helios::math::mat4f worldTransform_ = helios::math::mat4f::identity();


    public:

        /**
         * @brief Default constructor.
         */
        TransformComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        explicit TransformComponent(const TransformComponent& other) :
            transform_(other.transform_),
            worldTransform_(other.worldTransform_),
            isDirty_(other.isDirty_){
        }

        /**
         * @brief Checks if the transform is dirty.
         *
         * @return True if the transform has changed, false otherwise.
         */
        [[nodiscard]] bool isDirty() const noexcept {
            return isDirty_;
        }

        /**
         * @brief Clears the dirty flag.
         */
        void clearDirty() noexcept {
            isDirty_ = false;
        }

        /**
         * @brief Returns the local transformation matrix.
         *
         * @return The 4x4 matrix representing local translation, rotation, and scale.
         */
        [[nodiscard]] helios::math::mat4f localTransform() const noexcept {
            return transform_.transform();
        }


        /**
         * @brief Sets the local scale.
         *
         * @param scale The new scale vector.
         * @return Reference to this component for chaining.
         */
        TransformComponent& setLocalScale(const helios::math::vec3f& scale) noexcept {
            transform_.setScale(scale);
            isDirty_ = true;
            return *this;
        };


        /**
         * @brief Sets the local rotation matrix.
         *
         * @param rotation The new rotation matrix.
         * @return Reference to this component for chaining.
         */
        TransformComponent& setLocalRotation(const helios::math::mat4f& rotation) noexcept {
            transform_.setRotation(rotation);
            isDirty_ = true;
            return *this;
        };


        /**
         * @brief Sets the local translation (position).
         *
         * @param translation The new position vector.
         * @return Reference to this component for chaining.
         */
        TransformComponent& setLocalTranslation(const helios::math::vec3f& translation) noexcept {
            transform_.setTranslation(translation);
            isDirty_ = true;
            return *this;
        };

        /**
         * @brief Translates the component locally by a delta vector.
         *
         * @param translation The vector to add to the current position.
         * @return Reference to this component for chaining.
         */
        TransformComponent& translateLocalBy(const helios::math::vec3f& translation) noexcept {
            transform_.setTranslation(transform_.translation() + translation);
            isDirty_ = true;
            return *this;
        };

        /**
         * @brief Returns the local translation vector.
         *
         * @return The position relative to the parent.
         */
        [[nodiscard]] helios::math::vec3f localTranslation() const noexcept {
            return transform_.translation();
        }


        /**
         * @brief Returns the local rotation matrix.
         *
         * @return The rotation relative to the parent.
         */
        [[nodiscard]] helios::math::mat4f localRotation() const noexcept {
            return transform_.rotation();
        }


        /**
         * @brief Returns the local scaling vector.
         *
         * @return The scale relative to the parent.
         */
        [[nodiscard]] helios::math::vec3f localScaling() const noexcept {
            return transform_.scaling();
        }

        /**
         * @brief Sets the world transform matrix directly.
         *
         * @param m The new world transform matrix.
         */
        void setWorldTransform(const helios::math::mat4f& m) noexcept {
            isDirty_ = true;
            worldTransform_ = m;
        }

        /**
         * @brief Returns the current world transform matrix.
         *
         * @return Const reference to the world transform.
         */
        const helios::math::mat4f& worldTransform() noexcept {
            return worldTransform_;
        }

        /**
         * @brief Extracts the world translation from the world transform.
         *
         * @return The global position vector.
         */
        helios::math::vec3f worldTranslation() noexcept {
            return worldTransform_.translation();
        }
    };

}

