module;

#include <cassert>
#include <memory>
#include <vector>

module helios.scene.SceneNode;

import helios.rendering.Renderable;
import helios.util.Guid;
import helios.core.Transform;
import helios.math.types;
import helios.math.TransformType;

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


    SceneNode* SceneNode::addNode(std::unique_ptr<SceneNode> sceneNode) {
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



    SceneNode& SceneNode::setScale(const helios::math::vec3f& scale) noexcept {
        localTransform_.setScale(scale);
        needsUpdate_ = true;
        return *this;
    }


    SceneNode& SceneNode::setRotation(const helios::math::mat4f& rotation) noexcept {
        localTransform_.setRotation(rotation);
        needsUpdate_ = true;
        return *this;
    }


    SceneNode& SceneNode::setTranslation(const helios::math::vec3f& translation) noexcept {
        localTransform_.setTranslation(translation);
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


    bool SceneNode::applyWorldTransform(
        const helios::math::mat4f& parentWorldTransform, helios::scene::SceneGraphKey sceneGraphKey
        ) noexcept {

        const auto newWt = inheritWorldTransform(parentWorldTransform);

        if (!needsUpdate() && worldTransform_.same(newWt)) {
            return false;
        }

        worldTransform_ = newWt;
        needsUpdate_ = false;


        onWorldTransformUpdate();
        return true;
    }

    helios::math::mat4f SceneNode::inheritWorldTransform(const helios::math::mat4f& parentWorldTransform) noexcept {
        using namespace helios::math;

        if (inheritance_ == helios::math::TransformType::All) {
            return parentWorldTransform * localTransform_.transform();
        }

        auto id = parentWorldTransform.decompose(inheritance_);

        return id * localTransform_.transform();
    }


    const helios::math::mat4f& SceneNode::worldTransform() noexcept {
        if (needsUpdate()) {
            update();
        }

        return worldTransform_;
    }


    const helios::math::mat4f& SceneNode::cachedWorldTransform() const noexcept {
        return worldTransform_;
    }


    bool SceneNode::needsUpdate() const noexcept {
        return needsUpdate_ || localTransform_.needsUpdate();
    }

    helios::math::aabbf SceneNode::aabb() noexcept {
        if (needsUpdate()) {
            update();
        }

        return aabb_;
    }


    void SceneNode::update() noexcept {
        needsUpdate_ = false;

        if (parent_ == nullptr) {
            worldTransform_ = localTransform_.transform();
        } else {
            worldTransform_ = inheritWorldTransform(parent_->worldTransform());
        }

        onWorldTransformUpdate();

    }

    void SceneNode::onWorldTransformUpdate() noexcept {
        if (renderable_) {
            if (const auto prototype = renderable_->renderPrototype()) {
                const auto& localAABB = prototype->mesh().aabb();
                aabb_ = localAABB.applyTransform(worldTransform_);
            }
        }
    }

    helios::math::TransformType SceneNode::inheritance() const noexcept {
        return inheritance_;
    }

    void SceneNode::setInheritance(const helios::math::TransformType inherit) noexcept {
        inheritance_ = inherit;
        needsUpdate_ = true;
    }

    bool SceneNode::isActive() const noexcept {
        return isActive_;
    }

    void SceneNode::setActive(bool active) noexcept {
        isActive_ = active;
    }

    bool SceneNode::hasRenderable() const noexcept {
        return renderable() != nullptr;
    }

};

