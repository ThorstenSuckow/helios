module;

#include <iostream>
#include <memory>

module helios.scene.SnapshotItem;

import helios.rendering.Renderable;
import helios.math.types;

using namespace helios::rendering;
using namespace helios::math;

namespace helios::scene {


    SnapshotItem::SnapshotItem(
        std::weak_ptr<const Renderable> renderable,
        const mat4f& worldMatrix
    ) :
        renderable_(std::move(renderable)),
        worldMatrix_(worldMatrix) {
    }


    std::weak_ptr<const Renderable> SnapshotItem::renderable() const noexcept {
        return renderable_;
    }


    const mat4f& SnapshotItem::worldMatrix() const noexcept {
        return worldMatrix_;
    }
};

