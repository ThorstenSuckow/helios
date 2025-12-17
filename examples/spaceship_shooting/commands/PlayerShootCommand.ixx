/**
 * @file PlayerShootCommand.ixx
 * @brief Command for applying directional movement to a Spaceship's cannon.
 */
module;

export module helios.examples.spaceshipShooting.commands.PlayerShootCommand;

import helios.engine.game.Command;
import helios.engine.game.GameObject;
import helios.examples.spaceshipShooting.Spaceship;
import helios.math.types;
import helios.util.log;


#define HELIOS_LOG_SCOPE "helios::examples::spaceshipShooting::commands::PlayerShootCommand"
export namespace helios::examples::spaceshipShooting::commands {

    /**
     * @brief Command that applies directional movement to a Spaceship's cannon.
     *
     * @details This command encapsulates a movement request consisting of a normalized
     * direction vector and a "shooting frequency" factor derived from analog stick magnitude. When
     * executed, it invokes the Spaceship::fire() method to update cannon alignment and
     * fire frequency.
     *
     * The command uses dynamic_cast to safely convert the generic GameObject reference
     * to a Spaceship. If the cast fails, an error is logged and execution is skipped.
     *
     * @see helios::engine::game::Command
     * @see Spaceship::move
     */
    class PlayerShootCommand : public helios::engine::game::Command {

        /**
         * @brief Logger for this command type.
         */
        static inline const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief The analog stick magnitude determining movement intensity.
         */
        const float freqFactor_;

        /**
         * @brief The normalized direction vector for cannon movement.
         */
        const helios::math::vec2f direction_;

    public:

        /**
         * @brief Constructs a shoot command with direction and frequency.
         *
         * @param direction Normalized 2D direction vector from analog stick.
         * @param freqFactor Magnitude of the stick input (0.0 to 1.0).
         */
        explicit PlayerShootCommand(
            const helios::math::vec2f direction,
            const float freqFactor
        ) :
            direction_(direction),
            freqFactor_(freqFactor)
        {}

        /**
         * @brief Executes the movement command on a generic GameObject.
         *
         * @param gameObject The target entity, expected to be a Spaceship.
         *
         * @note Uses dynamic_cast to convert to Spaceship. If the cast fails,
         *       an error is logged and the command has no effect.
         */
        void execute(helios::engine::game::GameObject& gameObject) override {

            if (auto* ship = dynamic_cast<helios::examples::spaceshipShooting::Spaceship*>(&gameObject)) {
                execute(*ship);
            } else {
                logger_.error("Could not dynamic cast gameObject to Spaceship");
            }
        }

        /**
         * @brief Executes the shoot command directly on a Spaceship.
         *
         * @param spaceship The spaceship to apply movement to.
         */
        void execute(helios::examples::spaceshipShooting::Spaceship& spaceship) {

            spaceship.shoot(direction_, freqFactor_);

        }

    };


}