/**
 * @file LocalTransformComponent.ixx
 * @brief Component for managing local transformations of a GameObject.
 */
module;

export module helios.spatial.components.LocalTransformComponent;

import helios.math.types;

import helios.core.spatial;

export namespace helios::spatial::components {

    /**
     * @brief Component that holds transformation data (position, rotation, scale).
     *
     * @details The LocalTransformComponent manages the spatial state of a GameObject.
     * It stores the local transform (relative to parent) and the computed world transform.
     * Changes to local properties mark the component as dirty, signaling systems to
     * recompute the world transform.
     */
    template<typename THandle>
    class LocalTransformComponent {

        /**
         * @brief The local transformation (translation, rotation, scale).
         */
        helios::core::spatial::Transform transform_{};

        /**
         * @brief Flag indicating if the transform has changed since the last update.
         */
        bool isDirty_= true;

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        /**
         * @brief Default constructor.
         */
        LocalTransformComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        LocalTransformComponent(const LocalTransformComponent& other) :
            transform_(other.transform_),
            isDirty_(true){}

        LocalTransformComponent& operator=(const LocalTransformComponent&) = default;
        LocalTransformComponent(LocalTransformComponent&&) noexcept = default;
        LocalTransformComponent& operator=(LocalTransformComponent&&) noexcept = default;

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
         * @brief Resets the dirty flag to true when acquired.
         *
         * Makes sure the transform is considered after the component was acquired.
         */
        void onAcquire() noexcept {
            isDirty_ = true;
        }

        /**
         * @brief Resets the dirty flag to true when released.
         *
         * Makes sure the entities dirty-state is reset to the default state.
         */
        void onRelease() noexcept {
            isDirty_ = true;
        }

        /**
         * @brief Returns the local transformation matrix.
         *
         * @return The 4x4 matrix representing local translation, rotation, and scale.
         */
        [[nodiscard]] helios::math::mat4f transform() const noexcept {
            return transform_.transform();
        }


        /**
         * @brief Sets the local scale.
         *
         * @param scale The new scale vector.
         * @return Reference to this component for chaining.
         */
        LocalTransformComponent& setScale(const helios::math::vec3f& scale) noexcept {
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
        LocalTransformComponent& setRotation(const helios::math::mat4f& rotation) noexcept {
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
        LocalTransformComponent& setTranslation(const helios::math::vec3f& translation) noexcept {
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
        LocalTransformComponent& translateBy(const helios::math::vec3f& translation) noexcept {
            transform_.setTranslation(transform_.translation() + translation);
            isDirty_ = true;
            return *this;
        };

        /**
         * @brief Returns the local translation vector.
         *
         * @return The position relative to the parent.
         */
        [[nodiscard]] helios::math::vec3f translation() const noexcept {
            return transform_.translation();
        }


        /**
         * @brief Returns the local rotation matrix.
         *
         * @return The rotation relative to the parent.
         */
        [[nodiscard]] helios::math::mat4f rotation() const noexcept {
            return transform_.rotation();
        }


        /**
         * @brief Returns the local scaling vector.
         *
         * @return The scale relative to the parent.
         */
        [[nodiscard]] helios::math::vec3f scaling() const noexcept {
            return transform_.scaling();
        }

    };

}

