/**
 * @file ConsoleSink.ixx
 * @brief Log sink that writes to stdout.
 */
module;

#include <string>

export module helios.util.log.ConsoleSink;

import helios.util.log.LogSink;

export namespace helios::util::log {

    /**
     * @class ConsoleSink
     * @brief LogSink implementation that writes to standard output.
     *
     * This is the default sink for console-based logging. Output is formatted
     * with level prefix and scope tag, mirroring the original Logger behavior.
     */
    class ConsoleSink : public LogSink {

    public:
        /**
         * @brief Unique type identifier for this sink.
         */
        static constexpr SinkTypeId TYPE_ID = "console";

        /**
         * @brief Returns the unique type identifier for this sink.
         *
         * @return "console".
         */
        [[nodiscard]] SinkTypeId typeId() const noexcept override;

        /**
         * @brief Writes a formatted log message to stdout.
         *
         * @param level   The severity level of the message.
         * @param scope   The source scope/module name.
         * @param message The log message text.
         */
        void write(LogLevel level, const std::string& scope, const std::string& message) override;

        /**
         * @brief Flushes the stdout buffer.
         */
        void flush() override;
    };

}

