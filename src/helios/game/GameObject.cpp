module;

module helios.game.GameObject;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.scene.Transform;
import helios.math.types;
import helios.core.units;

namespace helios::game {

    GameObject::GameObject(helios::scene::SceneNode* sceneNode) :
        canonicalSize_(
            sceneNode->renderable() && sceneNode->renderable()->renderPrototype()
            ? sceneNode->renderable()->renderPrototype()->mesh().aabb().getSize()
            : helios::math::vec3f{1.0f, 1.0f, 1.0f}),
        transform_(&(sceneNode->localTransform())),
        guid_(helios::util::Guid::generate())
    {}

    const helios::util::Guid& GameObject::guid() const noexcept {
        return guid_;
    }

    helios::math::vec3f GameObject::position() const noexcept {
        return transform_->translation();
    }

    helios::math::vec2f GameObject::steeringInput() const noexcept {
        return steeringInput_;
    }

    float GameObject::throttle() const noexcept {
        return throttle_;
    }

    helios::math::vec3f GameObject::velocity() const noexcept {
        return velocity_;
    }

    float GameObject::speedRatio() const noexcept {
        return 0.0f;
    }

    GameObject& GameObject::setSize(
        const float width, const float height, const float depth, const helios::core::units::Unit unit
    ) noexcept {

        const auto& csize = canonicalSize_;

        transform_->setScale(
            helios::math::vec3f{
                width != 0 && csize[0] != 0 ? helios::core::units::from(width, unit)/csize[0] : 0,
                height != 0 && csize[0] != 0 ? helios::core::units::from(height, unit)/csize[1] : 0,
                depth != 0 && csize[0] != 0 ? helios::core::units::from(depth, unit)/csize[2] : 0,
            }
        );

        return *this;
    }

    GameObject& GameObject::setScale(const helios::math::vec3f& scale) noexcept {
        transform_->setScale(scale);
        return *this;
    }

    GameObject& GameObject::setRotation(const helios::math::mat4f& rotation) noexcept {
        transform_->setRotation(rotation);
        return *this;
    }

    GameObject& GameObject::setTranslation(const helios::math::vec3f& translation) noexcept {
        transform_->setTranslation(translation);
        return *this;
    }

} // namespace helios::game

