module;

#include <iostream>
#include <memory>
#include <vector>

export module helios.rendering.RenderQueue;

import helios.rendering.RenderCommand;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderQueue"
export namespace helios::rendering {

    /**
     * @brief Manages a collection of `RenderCommand` objects for a single rendering pass.
     *
     * `RenderQueue` acts as a container for all render commands, represented by a `RenderCommand`.
     * A `RenderQueue` holds unique ownership of the render commands.
     *
     * An instance of `RenderQueue` can be cleared after being processed in a `RenderPass`
     * for reuse in subsequent render passes.
     *
     * The class is designed to be non-copyable.
     *
     */
    class RenderQueue {

    protected:
        /**
         * @brief Stores the unique ptrs to the RenderCommand objects of this queue.
         */
        std::vector<std::unique_ptr<const helios::rendering::RenderCommand>> renderCommands_;

        /**
         * @brief Logger instance for this RenderQueue.
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);

    public:
        ~RenderQueue() = default;

        /**
         * @brief Delete copy constructor.
         */
        RenderQueue(const RenderQueue&) = delete;

        /**
         * @brief Delete copy assignment operator.
         */
        RenderQueue& operator=(const RenderQueue&) = delete;

        /**
         * @brief Delete move constructor.
         */
        RenderQueue(RenderQueue&&) noexcept = delete;

        /**
         * @brief Delete move assignment operator.
         */
        RenderQueue& operator=(RenderQueue&&) noexcept = delete;

        /**
         * @brief Constructs a new empty RenderQueue.
         */
        RenderQueue() = default;

        /**
         * @brief Adds a `RenderCommand` to this `RenderQueue`.
         * Ownership of the `RenderCommand` is transferred to this `RenderQueue`.
         *
         * @param renderCommand A unique_ptr to the RenderCommand. This
         * instance takes ownership of the RenderCommand.
         *
         * @todo prevent adding renderables while rendering
         */
        void add(std::unique_ptr<const helios::rendering::RenderCommand> renderCommand);

        /**
         * @brief Returns a const ref to the internal vector of `RenderableCommand`.
         *
         * @return A const ref to the list of `RenderCommand`s of this queue.
         */
        [[nodiscard]] const std::vector<std::unique_ptr<const helios::rendering::RenderCommand>>& renderCommands() const noexcept;

        /**
         * @brief Clears all `RenderableCommand` objects from the queue.
         *
         * This prepares this queue to be reused in a new rendering pass.
         */
        void clear();

        /**
         * @brief Returns the number of `RenderCommand`s this queue contains.
         *
         * @return The number of RenderCommands in this Queue.
         */
        [[nodiscard]] size_t count() const noexcept;
    };

}