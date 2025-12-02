module;

export module helios.game.GameObject;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.scene.Transform;
import helios.math.types;

export namespace helios::game {

    class GameObject {

    protected:


        helios::util::Guid guid_;

        /**
         * @todo needs to be notified BEFOE scenenode gets destroyed
         * to properly invalidate the pointer to teh SceneNode's transform
         */
        helios::scene::Transform* transform_;
    public:



        explicit GameObject(helios::scene::SceneNode* sceneNode) :
        transform_(&(sceneNode->localTransform())),
        guid_(helios::util::Guid::generate())
        {}

        virtual ~GameObject() = default;

        [[nodiscard]] const helios::util::Guid& guid() const noexcept {
            return guid_;
        }

        GameObject& scale(const helios::math::vec3f& scale) noexcept {
            transform_->scale(scale);
            return *this;
        }


        GameObject& rotate(const helios::math::mat4f& rotation) noexcept {
            transform_->rotate(rotation);
            return *this;
        }


        GameObject& translate(const helios::math::vec3f& translation) noexcept {
            transform_->translate(translation);
            return *this;
        }



    };


}