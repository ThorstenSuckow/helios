module;

#include <memory>
#include <vector>

module helios.scene.Snapshot;

import helios.scene.SnapshotItem;
import helios.math.types;
import helios.rendering.Viewport;

using namespace helios::math;
using namespace helios::rendering;

namespace helios::scene {

    Snapshot::Snapshot(
        std::shared_ptr<const Viewport> viewport,
        const mat4f& projectionMatrix,
        const mat4f& viewMatrix,
        std::vector<SnapshotItem> snapshotItems
    ) noexcept :
        viewport_(viewport),
        projectionMatrix_(projectionMatrix),
        viewMatrix_(viewMatrix),
        snapshotItems_(std::move(snapshotItems))
    {}

    std::shared_ptr<const helios::rendering::Viewport> Snapshot::viewport() const noexcept {
        return viewport_;
    }


    const std::vector<SnapshotItem>& Snapshot::snapshotItems() const noexcept {
        return snapshotItems_;
    }

    const mat4f& Snapshot::projectionMatrix() const noexcept {
        return projectionMatrix_;
    }

    const mat4f& Snapshot::viewMatrix() const noexcept {
        return viewMatrix_;
    }
};

