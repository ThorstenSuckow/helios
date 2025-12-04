module;

#include <memory>
#include <vector>

module helios.scene.SceneNode;

import helios.rendering.Renderable;
import helios.util.Guid;
import helios.scene.Transform;
import helios.math.types;


namespace helios::scene {


    void SceneNode::setParent(SceneNode* parentNode) {
        parent_ = parentNode;
    }



    SceneNode::SceneNode() noexcept :
                    guid_(util::Guid::generate())
    {}


    SceneNode::SceneNode(const Transform& transform) noexcept :
        guid_(util::Guid::generate()),
        localTransform_(transform)
    {}


    SceneNode::SceneNode(
        std::shared_ptr<helios::rendering::Renderable> renderable,
        const Transform& transform
    ) noexcept :
        guid_(util::Guid::generate()),
        renderable_(std::move(renderable)),
        localTransform_(transform)
    {}


    SceneNode::SceneNode(
                std::shared_ptr<helios::rendering::Renderable> renderable
            ) noexcept :
                guid_(util::Guid::generate()),
                renderable_(std::move(renderable))
    {}


    const helios::util::Guid& SceneNode::guid() const noexcept {
        return guid_;
    };


    SceneNode* SceneNode::addChild(std::unique_ptr<SceneNode> sceneNode) {
        auto& ref = *children_.emplace_back(std::move(sceneNode));
        ref.setParent(this);
        return &ref;
    }


    const std::vector<std::unique_ptr<SceneNode>>& SceneNode::children() const noexcept {
        return children_;
    }


    std::shared_ptr<const helios::rendering::Renderable> SceneNode::renderable() const noexcept {
        return renderable_;
    }


    std::shared_ptr<helios::rendering::Renderable> SceneNode::renderable() noexcept {
        return renderable_;
    }



    SceneNode& SceneNode::scale(const helios::math::vec3f& scale) noexcept {
        localTransform_.scale(scale);
        needsUpdate_ = true;
        return *this;
    }


    SceneNode& SceneNode::rotate(const helios::math::mat4f& rotation) noexcept {
        localTransform_.rotate(rotation);
        needsUpdate_ = true;
        return *this;
    }


    SceneNode& SceneNode::translate(const helios::math::vec3f& translation) noexcept {
        localTransform_.translate(translation);
        needsUpdate_ = true;
        return *this;
    }


    const Transform& SceneNode::localTransform() const noexcept {
        return localTransform_;
    }

    Transform& SceneNode::localTransform() noexcept {
        return localTransform_;
    }

    SceneNode* SceneNode::parent() const noexcept {
        return parent_;
    }


    bool SceneNode::setWorldTransform(
        const helios::math::mat4f& wf, helios::scene::SceneGraphKey sceneGraphKey
        ) noexcept {

        if (worldTransform_.same(wf)) {
            return false;
        }
        worldTransform_ = wf;
        needsUpdate_ = false;
        return true;
    }


    const helios::math::mat4f& SceneNode::worldTransform() noexcept {
        if (needsUpdate_) {
            needsUpdate_ = false;

            if (parent_ == nullptr) {
                worldTransform_ = localTransform_.transform();
            } else {
                worldTransform_= parent_->worldTransform() * localTransform_.transform();
            }
        }

        return worldTransform_;
    }


    const helios::math::mat4f& SceneNode::cachedWorldTransform() const noexcept {
        return worldTransform_;
    }


    bool SceneNode::needsUpdate() const noexcept {
        return needsUpdate_ || localTransform_.needsUpdate();
    }


};

