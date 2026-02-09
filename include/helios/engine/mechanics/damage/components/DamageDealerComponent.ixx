/**
 * @file DamageDealerComponent.ixx
 * @brief Component for defining layer-based damage values.
 */
module;

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <helios/helios_config.h>

export module helios.engine.mechanics.damage.components.DamageDealerComponent;

import helios.engine.core.data.CollisionLayer;





export namespace helios::engine::mechanics::damage::components {

    /**
     * @brief Component that stores damage values per collision layer.
     *
     * Allows an entity to deal different amounts of damage depending on
     * which collision layer the target belongs to. Uses a fixed-size array
     * indexed by layer ID for O(1) damage lookup.
     */
    class DamageDealerComponent {

    private:

        /**
         * @brief Damage values indexed by collision layer.
         */
        std::array<float, helios::engine::core::data::MAX_COLLISION_LAYERS> damageRegistry_{};

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        DamageDealerComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        DamageDealerComponent(const DamageDealerComponent& other) : damageRegistry_(other.damageRegistry_) {}

        DamageDealerComponent& operator=(const DamageDealerComponent&) = default;
        DamageDealerComponent(DamageDealerComponent&&) noexcept = default;
        DamageDealerComponent& operator=(DamageDealerComponent&&) noexcept = default;

        /**
         * @brief Sets the damage dealt to entities on a specific layer.
         *
         * @param damage The damage value to deal.
         * @param otherLayerId The target collision layer ID (must be a power of 2).
         *
         * @return Reference to this component for method chaining.
         */
        DamageDealerComponent& setDamage(
               const float damage,
               const uint32_t otherLayerId
        ) noexcept {
            const auto idx = std::countr_zero(otherLayerId);

            assert(idx != 0 && "Unexpected layer id, must not be 0");
            assert(idx < damageRegistry_.size() && "Unexpected otherLayerId, index larger than size");

            damageRegistry_[idx] = damage;
            return *this;
        }

        /**
         * @brief Returns the damage dealt to entities on a specific layer.
         *
         * @param otherLayerId The target collision layer ID (must be a power of 2).
         *
         * @return The damage value for the specified layer.
         */
        [[nodiscard]] float damage(const uint32_t otherLayerId) const noexcept {
            const auto idx = std::countr_zero(otherLayerId);

            assert(idx != 0 && "Unexpected layer id, must not be 0");
            assert(idx < damageRegistry_.size() && "Unexpected layer id, index larger than size");

            return damageRegistry_[idx];
        }

    };


}