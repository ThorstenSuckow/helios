/**
 * @file UpdateContext.ixx
 * @brief Context struct passed to components during per-frame updates.
 */
module;

#include <stdexcept>

export module helios.engine.game.UpdateContext;

import helios.engine.game.InputSnapshot;


export namespace helios::engine::game {

    class GameWorld;
    class CommandBuffer;

    /**
     * @brief Context passed to Updatable components during per-frame updates.
     *
     * @details Provides all necessary state for components to perform their update logic,
     * including frame timing, input state, and access to the command system.
     * All pointers are non-owning; the caller is responsible for lifetime management.
     */
    struct UpdateContext {

    private:
        /**
         * @brief Time elapsed since the last frame, in seconds.
         */
        float deltaTime_ = 0.0f;

        /**
         * @brief Immutable snapshot of input state for this frame.
         */
        const helios::engine::game::InputSnapshot* inputSnapshot_ = nullptr;

        /**
         * @brief Buffer for queueing commands to be executed at end of frame.
         */
        helios::engine::game::CommandBuffer* commandBuffer_;

        /**
         * @brief Reference to the game world for entity lookups.
         */
        helios::engine::game::GameWorld* gameWorld_;

    public:

        /**
         * @brief Constructs an UpdateContext with required dependencies.
         *
         * @param commandBuffer Non-owning pointer to the command buffer. Must not be nullptr.
         * @param gameWorld Non-owning pointer to the game world. Must not be nullptr.
         *
         * @throws std::invalid_argument if either commandBuffer or gameWorld are nullptr
         */
        UpdateContext(
            helios::engine::game::CommandBuffer* commandBuffer,
            helios::engine::game::GameWorld* gameWorld
        ) : commandBuffer_(commandBuffer), gameWorld_(gameWorld) {

            if (commandBuffer_ == nullptr || gameWorld_ == nullptr) {
                throw std::invalid_argument("UpdateContext received unexpected nullptr for commandBuffer or gameWorld.");
            }

        }

        /**
         * @brief Returns the time elapsed since the last frame, in seconds.
         */
        [[nodiscard]] float deltaTime() const noexcept {
            return deltaTime_;
        }

        /**
         * @brief Sets the time elapsed since the last frame.
         *
         * @param dt Delta time in seconds. Must be non-negative.
         *
         * @return A reference to this UpdateContext instance.
         */
        UpdateContext& setDeltaTime(float dt) noexcept {
            deltaTime_ = dt;

            return *this;
        }

        /**
         * @brief Returns the immutable input snapshot for this frame.
         *
         * @return Non-owning pointer. May be nullptr if no input is available.
         */
        [[nodiscard]] const helios::engine::game::InputSnapshot* inputSnapshot() const noexcept {
            return inputSnapshot_;
        }

        /**
         * @brief Sets the input snapshot for this frame.
         *
         * @param snapshot Non-owning pointer to the input snapshot.
         *
         * @return A reference to this UpdateContext instance.
         */
        UpdateContext& setInputSnapshot(const helios::engine::game::InputSnapshot* snapshot) noexcept {
            inputSnapshot_ = snapshot;

            return *this;
        }

        /**
         * @brief Returns the command buffer for queueing commands.
         *
         * @return Non-owning pointer. Guaranteed non-null after construction.
         */
        [[nodiscard]] helios::engine::game::CommandBuffer* commandBuffer() const noexcept {
            return commandBuffer_;
        }

        /**
         * @brief Returns the game world for entity lookups.
         *
         * @return Non-owning pointer. Guaranteed non-null after construction.
         */
        [[nodiscard]] helios::engine::game::GameWorld* gameWorld() const noexcept {
            return gameWorld_;
        }
    };
}