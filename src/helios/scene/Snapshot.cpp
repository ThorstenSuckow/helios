module;

#include <memory>
#include <vector>

module helios.scene.Snapshot;

import helios.scene.SnapshotItem;
import helios.math.types;

using namespace helios::math;

namespace helios::scene {

    Snapshot::Snapshot(
        const mat4f& projectionMatrix,
        const mat4f& viewMatrix,
        std::vector<SnapshotItem> snapshotItems
    ) noexcept :
        projectionMatrix_(projectionMatrix),
        viewMatrix_(viewMatrix),
        snapshotItems_(std::move(snapshotItems))
    {}

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

