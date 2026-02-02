/**
 * @file RenderQueue.ixx
 * @brief Container for enqueued render commands and renderables.
 */
module;

#include <cassert>
#include <format>
#include <helios/helios_config.h>
#include <memory>
#include <span>
#include <vector>

export module helios.rendering.RenderQueue;

import helios.rendering.mesh.MeshRenderCommand;
import helios.rendering.text.TextRenderCommand;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderQueue"
export namespace helios::rendering {

    /**
     * @brief Manages collections of render commands for geometry and text.
     *
     * `RenderQueue` acts as a container for all render commands in a single rendering pass.
     * It manages two separate command lists:
     *
     * - **RenderCommands:** For geometry rendering (meshes, materials).
     * - **TextRenderCommands:** For text rendering (glyphs, fonts).
     *
     * A `RenderQueue` holds unique ownership of geometry render commands and value ownership
     * of text render commands. It can be cleared after processing for reuse in subsequent passes.
     *
     * ## Design
     *
     * - **Non-copyable, non-movable:** Ensures unique queue identity during processing.
     * - **Separate command lists:** Allows different rendering strategies for geometry and text.
     *
     * @see RenderCommand
     * @see TextRenderCommand
     * @see RenderPass
     */
    class RenderQueue {

    protected:
        /**
         * @brief Stores the unique ptrs to the RenderCommand objects of this queue.
         */
        std::vector<helios::rendering::mesh::MeshRenderCommand> meshRenderCommands_;

        /**
         * @brief Stores text render commands for glyph-based text rendering.
         */
        std::vector<helios::rendering::text::TextRenderCommand> textRenderCommands_;

        /**
         * @brief Shared logger instance for all RenderQueue objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

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


        RenderQueue(RenderQueue&&) noexcept = default;


        RenderQueue& operator=(RenderQueue&&) noexcept = default;

        /**
         * @brief Constructs a new empty RenderQueue.
         */
        RenderQueue() {
            textRenderCommands_.reserve(RENDERQUEUE_TEXTRENDER_COMMANDS_SIZE);
            meshRenderCommands_.reserve(RENDERQUEUE_MESHRENDER_COMMANDS_SIZE);
        };

        /**
         * @brief Adds a `RenderCommand` to this `RenderQueue`.
         * Ownership of the `RenderCommand` is transferred to this `RenderQueue`.
         *
         * @param renderCommand A unique_ptr to the RenderCommand. This
         * instance takes ownership of the RenderCommand.
         *
         * @todo prevent adding renderables while rendering
         */
        void add(helios::rendering::mesh::MeshRenderCommand renderCommand) {
            meshRenderCommands_.emplace_back(std::move(renderCommand));
        }

        /**
         * @brief Adds a `TextRenderCommand` to this `RenderQueue`.
         *
         * The command is moved into the queue. Text commands are processed
         * separately from geometry commands during rendering.
         *
         * @param renderCommand The text render command to add (moved).
         */
        void add(helios::rendering::text::TextRenderCommand renderCommand) {
            textRenderCommands_.emplace_back(std::move(renderCommand));
        }

        /**
         * @brief Returns a const ref to the internal vector of `RenderCommand`.
         *
         * @return A const ref to the list of `RenderCommand`s of this queue.
         */
        [[nodiscard]] std::span<const helios::rendering::mesh::MeshRenderCommand> meshRenderCommands() const  noexcept {
            return meshRenderCommands_;
        }

        /**
         * @brief Returns a const ref to the internal vector of `TextRenderCommand`.
         *
         * @return A const ref to the list of text render commands in this queue.
         */
        [[nodiscard]] std::span<const helios::rendering::text::TextRenderCommand> textRenderCommands() const  noexcept {
            return textRenderCommands_;
        }

        /**
         * @brief Clears all `RenderCommand` objects from the queue.
         *
         * This prepares this queue to be reused in a new rendering pass.
         */
        void clear() {
            meshRenderCommands_.clear();
            textRenderCommands_.clear();

            /**
             * @todo strategy to decide whether shrink_to_fit should only be applied
             * if expected numbers RenderCommands for the subsequent render passes is
             * less than current size?
             */
            //renderCommands_.shrink_to_fit();
        }

        /**
         * @brief Returns the number of `RenderCommand`s this queue contains.
         *
         * @return The number of RenderCommands in this queue.
         */
        [[nodiscard]] size_t meshRenderCommandsSize() const noexcept {
            return meshRenderCommands_.size();
        }

        /**
         * @brief Returns the number of `TextRenderCommand`s this queue contains.
         *
         * @return The number of text render commands in this queue.
         */
        [[nodiscard]] size_t textRenderCommandsSize() const noexcept {
            return textRenderCommands_.size();
        }
    };

} // namespace helios::rendering