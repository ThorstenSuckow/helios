/**
 * @file LivesComponent.ixx
 * @brief Component tracking the remaining lives of an entity.
 */
module;

export module helios.engine.mechanics.match.components.LivesComponent;

import helios.engine.mechanics.match.types;

using namespace helios::engine::mechanics::match::types;

export namespace helios::engine::mechanics::match::components {

    /**
     * @brief Tracks the remaining lives of an entity.
     *
     * Each modification increments an internal LivesRevision counter so
     * that observer systems can detect changes without polling the value.
     */
    class LivesComponent {

        /**
         * @brief Current number of remaining lives.
         */
        size_t lives_;

        /**
         * @brief Monotonically increasing revision counter.
         */
        LivesRevision livesRevision_{1};

        public:

        /**
         * @brief Constructs a LivesComponent with an initial life count.
         *
         * @param lives Initial number of lives.
         */
        explicit LivesComponent(const size_t lives) noexcept : lives_(lives) {};

        /**
         * @brief Returns the current life count.
         *
         * @return Remaining lives.
         */
        [[nodiscard]] size_t lives() const noexcept {
            return lives_;
        }

        /**
         * @brief Decreases the life count by one and increments the revision.
         *
         * @return The new life count after decrement.
         */
        size_t decrease() noexcept {
            livesRevision_++;
            return --lives_;
        }

        /**
         * @brief Increases the life count by one and increments the revision.
         *
         * @return The new life count after increment.
         */
        size_t increase() noexcept {
            livesRevision_++;
            return ++lives_;
        }

        /**
         * @brief Returns the current lives revision.
         *
         * @return The LivesRevision value.
         */
        [[nodiscard]] LivesRevision livesRevision() const noexcept {
            return livesRevision_;
        }

    };

}