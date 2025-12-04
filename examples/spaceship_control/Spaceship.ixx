module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>

export module helios.examples.spaceshipControl.Spaceship;

import helios.game.GameObject;
import helios.math.types;
import helios.math.utils;
import helios.math.transform;
import helios.scene.SceneNode;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::examples::spaceshipControl::Spaceship"
export namespace helios::examples::spaceshipControl {

    class Spaceship : public helios::game::GameObject {

        static inline helios::util::log::Logger logger_ =
                helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);


        /**
         * The base rotation speed of this spaceship, in degrees per second
         */
        static constexpr float BASE_ROTATION_SPEED = 720.0f;


        static constexpr float MOVEMENT_SPEED_THRESHOLD = 0.1f;
        static constexpr float ROTATIONS_SPEED_THRESHOLD = 0.1f;

        /**
         * The base movement speed, in pixels per second.
         */
        static constexpr float BASE_MOVEMENT_SPEED = 1.50f;

        static constexpr float ROTATION_DAMPENING = 0.0001f;

        static constexpr float MOVEMENT_DAMPENING = 0.1f;

        /**
         * The current speed of the spaceship.
         */
        float actualMovementSpeed_ = 0.0f;


        float actualRotationAngle_ = 0;
        float targetRotationAngle_  = 0;

        float rotationAngleDelta_ = 0;
        float baseRotationSpeed = 16.0f;

        float actualRotationSpeed_ = 0.0f;
        float deadzone = 0.4f;

        helios::math::vec3f actualDirection_ = helios::math::vec3f(1.0f, 1.0f, 0);
        helios::math::vec3f actualPosition_ = helios::math::vec3f(0.0f, 0.0f, 0.0f);

        bool isInputActive_ = true;

    public:

        explicit Spaceship(helios::scene::SceneNode* sceneNode) : GameObject(sceneNode) {}

        void move(const helios::math::vec2f direction, const float speedFactor) {

            /**
             * @todo normalization needs to happen with the input manager
             */
            if (speedFactor <= deadzone) {
                isInputActive_ = false;
                return;
            }

            assert(std::abs(direction.length() - 1.0f) <= 0.001f && "Unexpected direction vector - not normalized");

            logger_.debug(std::format("Moving Spaceship w/ speedFactor {0}", speedFactor));

            targetRotationAngle_ = helios::math::degrees(std::atan2(direction[1], direction[0]));
            rotationAngleDelta_ = std::fmod((targetRotationAngle_ - actualRotationAngle_) + 540.0f, 360.0f) - 180.0f;


            isInputActive_ = true;

            float turnBoost = 1.0f + 0.5f*std::clamp((abs(rotationAngleDelta_))/180.f, 0.0f, 1.0f);
            actualRotationSpeed_ = turnBoost * BASE_ROTATION_SPEED * speedFactor;
            actualMovementSpeed_ = BASE_MOVEMENT_SPEED * speedFactor;


        }



        void update(const float DELTA_TIME) override {

            logger_.debug(std::format("Updating at {0}", DELTA_TIME));

            assert(DELTA_TIME >= 0 && "Unexpected negative value for DELTA_TIME");

            if (DELTA_TIME == 0) {
                logger_.debug("DELTA_TIME was 0, skipping update()");
                return;
            }

            // ROTATION
            if (!isInputActive_) {
                actualRotationSpeed_ *= std::pow(ROTATION_DAMPENING, DELTA_TIME);;
                if (actualRotationSpeed_ < ROTATIONS_SPEED_THRESHOLD) {
                    actualRotationSpeed_ = 0.0f;
                }
            }

            if (actualRotationSpeed_ > 0.0f) {
                float rotationStep = std::copysign(
                 std::min(std::abs(rotationAngleDelta_), actualRotationSpeed_ * DELTA_TIME),
                 rotationAngleDelta_
                );
                actualRotationAngle_ += rotationStep;

                rotationAngleDelta_ = std::fmod((targetRotationAngle_ - actualRotationAngle_) + 540.0f, 360.0f) - 180.0f;

                if (std::abs(rotationAngleDelta_) <= 0.5f) {
                    actualRotationAngle_ = targetRotationAngle_;
                    rotationAngleDelta_  = 0.0f;
                    actualRotationSpeed_ = 0.0f;
                }

                rotate(helios::math::rotate(
                helios::math::mat4f::identity(),
                helios::math::radians(actualRotationAngle_),
                helios::math::vec3f(0.0f, 0.0f, 1.0f)
                ));


            }

            // MOVEMENT
            if (!isInputActive_) {
                actualMovementSpeed_ *= std::pow(MOVEMENT_DAMPENING, DELTA_TIME);

                if (actualMovementSpeed_ < MOVEMENT_SPEED_THRESHOLD) {
                    actualMovementSpeed_ = 0.0f;
                }
            }

            if (actualMovementSpeed_ > 0.0f) {
                actualDirection_ = helios::math::vec3f(
                    cos(helios::math::radians(actualRotationAngle_)), sin(helios::math::radians(actualRotationAngle_)), 0.0f
                );

                actualPosition_ = actualPosition_ + (actualDirection_ * actualMovementSpeed_ * DELTA_TIME);
                logger_.debug(std::format("Updating ship with position ({0},{1},{2}), speed {3}", actualPosition_[0], actualPosition_[1], actualPosition_[2], actualMovementSpeed_));
                translate(actualPosition_);

            }


        }



            

        

    };

}
