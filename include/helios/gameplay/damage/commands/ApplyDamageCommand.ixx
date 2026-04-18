/**
 * @file ApplyDamageCommand.ixx
 * @brief Command for requesting damage application to an entity.
 */
module;

export module helios.gameplay.damage.commands.ApplyDamageCommand;

import helios.engine.common.types;


using namespace helios::engine::common::types;
export namespace helios::gameplay::damage::commands {

    /**
     * @brief Command that carries a DamageContext for deferred damage processing.
     *
     * Issued by damage-producing systems (e.g. DamageOnCollisionSystem) and
     * submitted to the HealthManager via the TypedCommandHandler interface
     * during the command flush phase.
     *
     * @see DamageContext
     * @see HealthManager
     */
    template<typename THandle>
    class ApplyDamageCommand {

        /**
         * @brief The damage context describing target, instigator, and amount.
         */
        DamageContext<THandle> damageContext_;

    public:

        /**
         * @brief Constructs a command from a damage context.
         *
         * @param damageContext The context describing the damage to apply.
         */
        explicit ApplyDamageCommand(const DamageContext<THandle>& damageContext)
        : damageContext_(damageContext) {}

        /**
         * @brief Returns the damage context.
         *
         * @return The stored DamageContext.
         */
        [[nodiscard]] auto damageContext() const noexcept {
            return damageContext_;
        }

    };


}