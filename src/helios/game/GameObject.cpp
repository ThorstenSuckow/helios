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

        transform_->scale(
            helios::math::vec3f{
                helios::core::units::to(width != 0 && csize[0] != 0 ? width/csize[0] : 0, unit),
                helios::core::units::to(height != 0 && csize[1] != 0 ? height/csize[1] : 0, unit),
                helios::core::units::to(depth != 0 && csize[2] != 0 ? depth/csize[2] : 0, unit),
            }
        );

        return *this;
    }

    GameObject& GameObject::scale(const helios::math::vec3f& scale) noexcept {
        transform_->scale(scale);
        return *this;
    }

    GameObject& GameObject::rotate(const helios::math::mat4f& rotation) noexcept {
        transform_->rotate(rotation);
        return *this;
    }

    GameObject& GameObject::translate(const helios::math::vec3f& translation) noexcept {
        transform_->translate(translation);
        return *this;
    }

} // namespace helios::game

