/**
 * @file CommandBufferRole.ixx
 * @brief Tag type for identifying CommandBuffer-role classes at compile time.
 */
module;

export module helios.runtime.messaging.command.tags.CommandBufferRole;

export namespace helios::runtime::tags {

    /**
     * @brief Compile-time tag identifying a class as a CommandBuffer.
     *
     * @see IsCommandBufferLike
     * @see HasTag
     */
    struct CommandBufferRole{};
}