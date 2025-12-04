/**
 * @file Logger.ixx
 * @brief Simple synchronous logger with configurable output sinks.
 */
module;

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

export module helios.util.log.Logger;

import helios.util.log.LogSink;

export namespace helios::util::log {

    /**
     * @brief Logger implementation with configurable output sinks.
     *
     * This logger supports multiple output destinations through LogSink instances.
     * By default, it writes to stdout, but sinks can be added or replaced to redirect
     * output to ImGui widgets, files, or other destinations.
     *
     * @code
     * // Add ImGui sink while keeping console output
     * logger.addSink(imguiSink);
     *
     * // Replace all sinks (ImGui only)
     * logger.clearSinks();
     * logger.addSink(imguiSink);
     * @endcode
     */
    class Logger {

    private:
        const std::string scope_ = "default";

        /**
         * @brief Flag to indicate whether this Logger's output is enabled.
         */
        bool enabled_ = true;

        /**
         * @brief Collection of output sinks.
         */
        std::vector<std::shared_ptr<LogSink>> sinks_;

        /**
         * @brief Mutex for thread-safe sink access.
         */
        mutable std::mutex sinkMutex_;

        /**
         * @brief Dispatches a message to all registered sinks.
         */
        void dispatch(LogLevel level, const std::string& msg) const noexcept {
            if (!enabled_) return;

            std::lock_guard<std::mutex> lock(sinkMutex_);
            if (sinks_.empty()) {
                // Fallback to stdout if no sinks configured
                const char* levelStr = "";
                switch (level) {
                    case LogLevel::Debug: levelStr = "[DEBUG]"; break;
                    case LogLevel::Info:  levelStr = "[INFO]";  break;
                    case LogLevel::Warn:  levelStr = "[WARN]";  break;
                    case LogLevel::Error: levelStr = "[ERROR]"; break;
                }
                std::cout << levelStr << "[" << scope_ << "] " << msg << std::endl;
            } else {
                for (const auto& sink : sinks_) {
                    if (sink) {
                        sink->write(level, scope_, msg);
                    }
                }
            }
        }

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
         * @brief Adds an output sink to this logger.
         *
         * @param sink Shared pointer to the sink to add.
         */
        void addSink(std::shared_ptr<LogSink> sink) {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            sinks_.push_back(std::move(sink));
        }

        /**
         * @brief Removes all sinks from this logger.
         */
        void clearSinks() noexcept {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            sinks_.clear();
        }

        /**
         * @brief Returns the number of attached sinks.
         */
        [[nodiscard]] size_t sinkCount() const noexcept {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            return sinks_.size();
        }

        /**
         * @brief Writes a warning message if logging is enabled.
         *
         * @param msg The message to write.
         */
        void warn(const std::string& msg) const noexcept {
            dispatch(LogLevel::Warn, msg);
        }


        /**
         * @brief Writes a debug message if logging is enabled.
         *
         * @param msg The message to write.
         */
        void debug(const std::string& msg) const noexcept {
            dispatch(LogLevel::Debug, msg);
        }


        /**
         * @brief Writes an info message if logging is enabled.
         *
         * @param msg The message to write.
         */
        void info(const std::string& msg) const noexcept {
            dispatch(LogLevel::Info, msg);
        }


        /**
         * @brief Writes an error message if logging is enabled.
         *
         * @param msg The message to write.
         */
        void error(const std::string& msg) const noexcept {
            dispatch(LogLevel::Error, msg);
        }

    };


}