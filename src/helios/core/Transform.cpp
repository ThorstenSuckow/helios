module;

module helios.core.spatial.Transform;

import helios.math.types;
import helios.math.transform;


namespace helios::core::spatial {

    helios::math::mat4f Transform::updateCache() const {
        needsUpdate_ = false;
        return math::translate(
            helios::math::mat4f::identity(),
            translation_)  *
            (rotation_ * helios::math::mat4f(scale_)
        );
    }

    Transform::Transform(const helios::math::mat4f& rotation,
        helios::math::vec3f scale,
        helios::math::vec3f translation
    ) noexcept :
        rotation_(rotation),
        scale_(scale),
        translation_(translation),
        needsUpdate_(true)
    {}


    void Transform::setRotation(const math::mat4f& rotation) noexcept {
        rotation_ = rotation;
        needsUpdate_ = true;
    }


    void Transform::setTranslation(const math::vec3f& translation) noexcept {
        translation_ = translation;
        needsUpdate_ = true;
    }


    void Transform::setScale(const math::vec3f& scale) noexcept {
        scale_ = scale;
        needsUpdate_ = true;
    }


    helios::math::mat4f Transform::rotation() const noexcept {
        return rotation_;
    }


    helios::math::vec3f Transform::translation() const noexcept {
        return translation_;
    }


    helios::math::vec3f Transform::scaling() const noexcept {
        return scale_;
    }


    helios::math::mat4f Transform::transform() const noexcept {
        if (needsUpdate_) {
            cached_ = updateCache();
        }
        return cached_;
    }

    bool Transform::needsUpdate() const noexcept {
        return needsUpdate_;
    }


};