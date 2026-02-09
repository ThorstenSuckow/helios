/**
 * @file RotationStateComponent.ixx
 * @brief Component for managing composite rotation state from heading and spin.
 */
module;

#include <cmath>

export module helios.engine.modules.spatial.transform.components.RotationStateComponent;



import helios.math;

/**
 * @todo move to spatial transform
 */
export namespace helios::engine::modules::spatial::transform::components {

    /**
     * @brief Component that manages composite rotation from heading and spin rotations.
     *
     * @details
     * This component combines two independent rotation sources:
     * - **Heading Rotation:** The direction the entity is facing (controlled by input).
     * - **Spin Rotation:** Additional rotation for visual effects (e.g., rolling, tumbling).
     *
     * The component caches the computed rotation matrices and only recalculates them
     * when the underlying angles or axes change (dirty flag pattern).
     */
    class RotationStateComponent  {

        /**
         * @brief Current heading rotation angle in degrees.
         */
        float headingRotationAngle_ = 0.0f;

        /**
         * @brief Axis around which heading rotation occurs.
         */
        helios::math::vec3f headingRotationAxis_{};

        /**
         * @brief Current spin rotation angle in degrees.
         */
        float spinRotationAngle_ = 0.0f;

        /**
         * @brief Axis around which spin rotation occurs.
         */
        helios::math::vec3f spinRotationAxis_{};

        /**
         * @brief Dirty flag indicating matrices need recalculation.
         */
        bool needsUpdate_ = true;

        /**
         * @brief Cached spin rotation matrix.
         */
        helios::math::mat4f spinRotationMatrix_;

        /**
         * @brief Cached heading rotation matrix.
         */
        helios::math::mat4f headingRotationMatrix_;

        /**
         * @brief Cached composed rotation matrix (heading * spin).
         */
        helios::math::mat4f composedRotationMatrix_;

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

    private:

        /**
         * @brief Recalculates rotation matrices if dirty.
         *
         * @details Updates spinRotationMatrix_, headingRotationMatrix_, and
         * composedRotationMatrix_ based on current angles and axes.
         */
        void update() {

            if (!needsUpdate_) {
                return;
            }
            needsUpdate_ = false;

            spinRotationMatrix_ =  helios::math::rotate(
            helios::math::mat4f::identity(),
            helios::math::radians(spinRotationAngle_),
            spinRotationAxis_
            );

            headingRotationMatrix_ =  helios::math::rotate(
                helios::math::mat4f::identity(),
                helios::math::radians(headingRotationAngle_),
                headingRotationAxis_

            );

            composedRotationMatrix_ = headingRotationMatrix_ * spinRotationMatrix_;
        }

    public:

        /**
         * @brief Default constructor.
         */
        RotationStateComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         *
         * @details Copies rotation axes but resets angles and matrices.
         */
        RotationStateComponent(const RotationStateComponent& other) :
        headingRotationAxis_(other.headingRotationAxis_),
        spinRotationAxis_(other.spinRotationAxis_)
        {}

        RotationStateComponent& operator=(const RotationStateComponent&) = default;
        RotationStateComponent(RotationStateComponent&&) noexcept = default;
        RotationStateComponent& operator=(RotationStateComponent&&) noexcept = default;

        /**
         * @brief Sets the heading rotation angle.
         *
         * @param angle The new heading rotation angle in degrees (wrapped to 0-360).
         */
        void setHeadingRotationAngle(float angle) {
            headingRotationAngle_ = std::fmod(angle, 360.0f);
            needsUpdate_ = true;
        }

        /**
         * @brief Returns the current heading rotation angle.
         *
         * @return The heading rotation angle in degrees.
         */
        [[nodiscard]] float headingRotationAngle() const noexcept {
            return headingRotationAngle_;
        }

        /**
         * @brief Sets the heading rotation axis.
         *
         * @param axis The new rotation axis (should be normalized).
         */
        void setHeadingRotationAxis(helios::math::vec3f axis) {
            headingRotationAxis_ = axis;
            needsUpdate_ = true;
        }

        /**
         * @brief Returns the heading rotation axis.
         *
         * @return The current heading rotation axis.
         */
        [[nodiscard]] helios::math::vec3f headingRotationAxis() const noexcept {
            return headingRotationAxis_;
        }

        /**
         * @brief Sets the spin rotation angle.
         *
         * @param angle The new spin rotation angle in degrees (wrapped to 0-360).
         */
        void setSpinRotationAngle(float angle) {
            spinRotationAngle_ =  std::fmod(angle, 360.0f);
            needsUpdate_ = true;
        }

        /**
         * @brief Returns the current spin rotation angle.
         *
         * @return The spin rotation angle in degrees.
         */
        [[nodiscard]] float spinRotationAngle() const noexcept {
            return spinRotationAngle_;
        }

        /**
         * @brief Sets the spin rotation axis.
         *
         * @param axis The new spin rotation axis (should be normalized).
         */
        void setSpinRotationAxis(helios::math::vec3f axis) {
            spinRotationAxis_ = axis;
            needsUpdate_ = true;
        }

        /**
         * @brief Returns the spin rotation axis.
         *
         * @return The current spin rotation axis.
         */
        [[nodiscard]] helios::math::vec3f spinRotationAxis() const noexcept {
            return spinRotationAxis_;
        }

        /**
         * @brief Returns the spin rotation matrix.
         *
         * @details Triggers matrix update if dirty.
         *
         * @return Const reference to the spin rotation matrix.
         */
        const helios::math::mat4f& spinRotation() noexcept {
            update();
            return spinRotationMatrix_;
        }

        /**
         * @brief Returns the heading rotation matrix.
         *
         * @details Triggers matrix update if dirty.
         *
         * @return Const reference to the heading rotation matrix.
         */
        const helios::math::mat4f& headingRotation() noexcept {
            update();
            return headingRotationMatrix_;
        }

        /**
         * @brief Returns the composed rotation matrix (heading * spin).
         *
         * @details Triggers matrix update if dirty.
         *
         * @return Const reference to the composed rotation matrix.
         */
        const helios::math::mat4f& rotation() noexcept {
            update();
            return composedRotationMatrix_;
        }

    };

};

