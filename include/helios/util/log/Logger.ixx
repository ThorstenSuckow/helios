/**
 * @file Logger.ixx
 * @brief Simple synchronous logger that writes messages to stdout.
 */
module;

#include <iostream>
#include <string>

export module helios.util.log.Logger;

export namespace helios::util::log {

    /**
     * @brief Simple Logger implementation.
     *
     * This logger writes messages to standard output with a scope prefix.
     * It is intended for lightweight diagnostic output in examples and tests.
     *
     * @todo configure log stream, severity filtering and thread-safety.
     */
    class Logger {

    private:
        const std::string scope_ = "default";

        /**
         * @brief Flag to indicate whether this Logger's output is enabled.
         */
        bool enabled_ = true;

    public:
        /**
         * @brief Creates a new Logger, tagged with a specific scope.
         *
         * @param scope The textual scope used as a prefix in log output (e.g. "helios::rendering").
         */
        explicit Logger(std::string scope) :
            scope_(std::move(scope)) {}


        /**
         * @brief Enables or disables log output for this Logger instance.
         *
         * @param enable true to enable output, false to disable it.
         */
        void enable(const bool enable) noexcept {
            enabled_ = enable;
        }

        /**
         * @brief Writes a warning message to stdout if logging is enabled.
         *
         * @param msg The message to write.
         */
        void inline warn(const std::string& msg) const noexcept{
            enabled_ && std::cout << "[WARN]" << "[" << scope_ << "] " << msg << std::endl;
        }


        /**
         * @brief Writes a debug message to stdout if logging is enabled.
         *
         * @param msg The message to write.
         */
        void inline debug(const std::string& msg) const noexcept {
            enabled_ && std::cout << "[DEBUG]" << "[" << scope_ << "] " << msg << std::endl;
        }


        /**
         * @brief Writes an info message to stdout if logging is enabled.
         *
         * @param msg The message to write.
         */
        void inline info(const std::string& msg)  const noexcept{
            enabled_ && std::cout << "[INFO]" << "[" << scope_ << "] " << msg << std::endl;
        }


        /**
         * @brief Writes an error message to stdout if logging is enabled.
         *
         * @param msg The message to write.
         */
        void inline error(const std::string& msg) const noexcept {
            enabled_ && std::cout << "[ERROR]" << "[" << scope_ << "] " << msg << std::endl;
        }

    };


}