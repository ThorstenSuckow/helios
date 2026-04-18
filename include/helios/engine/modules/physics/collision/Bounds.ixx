/**
 * @file Bounds.ixx
 * @brief Utility functions for computing world-space bounding volumes.
 */
module;

export module helios.engine.modules.physics.collision.Bounds;


import helios.math;

import helios.scene.components.SceneNodeComponent;
import helios.engine.modules.spatial.transform.components.ScaleStateComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

export namespace helios::engine::modules::physics::collision::Bounds {

    /**
     * @brief Computes the world-space AABB from local model bounds and transforms.
     *
     * Combines the parent's world transform with the entity's local translation,
     * rotation, and scale to transform the model-space AABB into world space.
     *
     * The transformation order is: Translation * Rotation * Scale (TRS).
     *
     * @param mab Model AABB component containing the local bounding box.
     * @param sc Scene node component for accessing parent world transform.
     * @param tsc Translation state component with local position.
     * @param sca Scale state component with local scaling factors.
     * @param rsc Rotation state component with local rotation matrix.
     *
     * @return World-space axis-aligned bounding box.
     */
    template<typename THandle>
    inline helios::math::aabbf computeWorldAabb(
        const helios::engine::modules::rendering::model::components::ModelAabbComponent<THandle>& mab,
        const helios::scene::components::SceneNodeComponent<THandle>& sc,
        const helios::engine::modules::spatial::transform::components::TranslationStateComponent<THandle>& tsc,
        const helios::engine::modules::spatial::transform::components::ScaleStateComponent<THandle>& sca,
        helios::engine::modules::spatial::transform::components::RotationStateComponent<THandle>& rsc
    ) noexcept  {

        const helios::math::mat4f& parentTransform = sc.sceneNode()->parent()->worldTransform();

        const helios::math::vec3f localTranslation = tsc.translation();
        const helios::math::mat4f localRotation    = rsc.rotation();
        const helios::math::vec3f localScaling     = sca.scaling();

        const helios::math::mat4f mScaling     = helios::math::mat4f::identity().withScaling(localScaling);
        const helios::math::mat4f mTranslation = helios::math::mat4f::identity().withTranslation(localTranslation);

        const helios::math::mat4f localTransform =  mTranslation * (localRotation * mScaling);

        return mab.aabb().applyTransform(parentTransform * localTransform);
    }

}
