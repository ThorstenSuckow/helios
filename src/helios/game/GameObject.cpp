module;

module helios.game.GameObject;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.scene.Transform;
import helios.math.types;

namespace helios::game {

    GameObject::GameObject(helios::scene::SceneNode* sceneNode) :
        transform_(&(sceneNode->localTransform())),
        guid_(helios::util::Guid::generate())
    {}

    const helios::util::Guid& GameObject::guid() const noexcept {
        return guid_;
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

