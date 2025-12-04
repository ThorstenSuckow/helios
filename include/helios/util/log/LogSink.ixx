/**
 * @file LogSink.ixx
 * @brief Abstract interface for log output destinations.
 */
module;

#include <string>

export module helios.util.log.LogSink;

export namespace helios::util::log {

    /**
     * @brief Severity level for log messages.
     */
    enum class LogLevel {
        Debug,
        Info,
        Warn,
        Error
    };

    /**
     * @brief Unique identifier type for log sinks.
     *
     * Each sink implementation defines its own static identifier string.
     * The LogManager uses these identifiers to enable/disable specific sinks.
     *
     * ```cpp
     * // In sink implementation:
     * static constexpr const char* TYPE_ID = "console";
     * [[nodiscard]] const char* typeId() const noexcept override { return TYPE_ID; }
     *
     * // Usage:
     * LogManager::getInstance().enableSink("console");
     * LogManager::getInstance().disableSink("imgui");
     * ```
     */
    using SinkTypeId = const char*;

    /**
     * @interface LogSink
     * @brief Abstract sink interface for log message output.
     *
     * LogSink defines the contract for log output destinations. Implementations
     * can write to console, files, ImGui widgets, network sockets, or any other
     * target. Multiple sinks can be attached to a Logger for simultaneous output
     * to different destinations.
     *
     * Each sink must provide a unique type identifier via `typeId()`. This allows
     * the LogManager to selectively enable or disable sinks at runtime without
     * requiring a central registry of all possible sink types.
     */
    class LogSink {

    public:
        virtual ~LogSink() = default;

        /**
         * @brief Returns the unique type identifier for this sink.
         *
         * Each sink implementation should define a static constexpr identifier
         * and return it here. Used by LogManager to match sinks for enable/disable.
         *
         * @return A string identifier unique to this sink type (e.g., "console", "file").
         */
        [[nodiscard]] virtual SinkTypeId typeId() const noexcept = 0;

        /**
         * @brief Writes a log message to this sink.
         *
         * @param level   The severity level of the message.
         * @param scope   The source scope/module name.
         * @param message The log message text.
         */
        virtual void write(LogLevel level, const std::string& scope, const std::string& message) = 0;

        /**
         * @brief Flushes any buffered output (optional).
         *
         * Default implementation does nothing. Override for sinks that buffer.
         */
        virtual void flush() {}
    };

}

