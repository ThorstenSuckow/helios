/**
 * @file LivesBindingComponent.ixx
 * @brief Component binding a UI element to a LivesComponent on another entity.
 */
module;

export module helios.gameplay.match.components.LivesBindingComponent;

import helios.gameplay.match.types;

import helios.ecs;
import helios.core.types;

using namespace helios::ecs::types;
using namespace helios::gameplay::match::types;

export namespace helios::gameplay::match::components {

    /**
     * @brief Binds a UI entity to the LivesComponent of a target entity.
     *
     * Stores the target EntityHandle and a local LivesRevision snapshot.
     * Observer systems compare the stored revision against the target's
     * LivesComponent revision to detect changes and update the UI.
     */
    template<typename THandle>
    class LivesBindingComponent {

    private:

        /**
         * @brief Handle of the entity whose lives are observed.
         */
        THandle entityHandle_;

        /**
         * @brief Last-seen lives revision for change detection.
         */
        LivesRevision livesRevision_;

    public:

        /**
         * @brief Constructs a LivesBindingComponent.
         *
         * @param target Handle of the entity to observe.
         */
        LivesBindingComponent(THandle target) : entityHandle_(target) {};

        /**
         * @brief Returns the observed entity handle.
         *
         * @return The target THandle.
         */
        [[nodiscard]] THandle target() const noexcept {
            return entityHandle_;
        }

        /**
         * @brief Updates the stored lives revision.
         *
         * @param livesRevision New revision value.
         */
        void setLivesRevision(const LivesRevision livesRevision) noexcept {
            livesRevision_ = livesRevision;
        }

        /**
         * @brief Returns the stored lives revision.
         *
         * @return The current LivesRevision snapshot.
         */
        [[nodiscard]] LivesRevision livesRevision() const noexcept {
            return livesRevision_;
        }

    };


}