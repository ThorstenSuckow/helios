/**
 * @file LogManager.ixx
 * @brief Central manager for scoped loggers with configurable sinks.
 */
module;

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <vector>
#include <cstring>

export module helios.util.log.LogManager;

import helios.util.log.LogSink;
import helios.util.log.Logger;

#define LOGGING_ENABLED true
export namespace helios::util::log {

    /**
     * @brief LogManager for managing scoped Loggers and global sink configuration.
     *
     * The LogManager provides centralized control over logging output destinations.
     * Sinks register themselves with a unique type identifier, allowing dynamic
     * enable/disable without compile-time knowledge of all sink types.
     *
     * ```cpp
     * // Register sinks (sinks define their own TYPE_ID)
     * LogManager::getInstance().registerSink(std::make_shared<ConsoleSink>());
     * LogManager::getInstance().registerSink(std::make_shared<ImGuiLogSink>(widget));
     *
     * // Enable/disable by type identifier
     * LogManager::getInstance().enableSink("console");
     * LogManager::getInstance().disableSink("imgui");
     *
     * // Check status
     * if (LogManager::getInstance().isSinkEnabled("console")) { ... }
     * ```
     */
    class LogManager {

    private:

        /**
         * @brief Flag indicating whether log output should be globally enabled or disabled.
         */
        bool loggingEnabled_ = LOGGING_ENABLED;

        /**
         * @brief Set of currently enabled sink type identifiers.
         */
        std::unordered_set<std::string> enabledSinks_;

        /**
         * @brief Registered sinks (all available sinks, regardless of enabled state).
         */
        std::vector<std::shared_ptr<LogSink>> registeredSinks_;

        /**
         * @brief Unordered map holding unique pointers to the loggers managed
         * by this class, guaranteed to be not null.
         */
        std::unordered_map<std::string, std::unique_ptr<Logger>> loggers_;

        /**
         * @brief Default logger if a logger for a specific scope was not found.
         */
        const std::unique_ptr<Logger> defaultLogger_;

        /**
         * @brief Mutex providing mutually exclusive access to the loggers map.
         */
        mutable std::mutex mapMutex_;

        /**
         * @brief Mutex for sink access.
         */
        mutable std::mutex sinkMutex_;

        /**
         * @brief Creates the LogManager and registers an unscoped default logger.
         */
        LogManager() : defaultLogger_(std::make_unique<Logger>("default")) {}

        /**
         * @brief Reconfigures all logger sinks based on currently enabled sinks.
         *
         * Called internally after sink enable/disable changes.
         */
        void updateLoggerSinks() {
            // Called with sinkMutex_ already held
            std::lock_guard<std::mutex> mapLock(mapMutex_);

            // Update default logger
            defaultLogger_->clearSinks();
            for (const auto& sink : registeredSinks_) {
                if (sink && enabledSinks_.contains(sink->typeId())) {
                    defaultLogger_->addSink(sink);
                }
            }

            // Update all registered loggers
            for (auto& [scope, logger] : loggers_) {
                logger->clearSinks();
                for (const auto& sink : registeredSinks_) {
                    if (sink && enabledSinks_.contains(sink->typeId())) {
                        logger->addSink(sink);
                    }
                }
            }
        }

    public:

        /**
         * @brief Convenience accessor to obtain a logger for a textual scope via the singleton.
         *
         * This static helper forwards to `LogManager::getInstance().logger(scope)` and
         * returns a reference to the logger registered for the given scope.
         *
         * @param scope The textual scope name of the requested logger.
         *
         * @return The logger registered with the scope, or the default logger if none was found.
         */
        static const Logger& loggerForScope(const std::string& scope) noexcept {
            return LogManager::getInstance().registerLogger(scope);
        }

        ~LogManager() = default;

        /**
         * Enforce singleton (see Meyer's Singleton)
         *
         * @see https://en.wikipedia.org/wiki/Singleton_pattern
         */
        LogManager(const LogManager&) = delete;
        LogManager& operator=(const LogManager&) = delete;

        /**
         * @brief Returns the LogManager singleton instance.
         *
         * @return Reference to the global LogManager instance.
         */
        static LogManager& getInstance() noexcept {
            static LogManager instance;

            return instance;
        }

        /**
         * @brief Returns a const reference to the default logger managed with this LogManager.
         *
         * @return The default Logger instance.
         */
        [[nodiscard]] const Logger& logger() const noexcept {
            return *defaultLogger_;
        }

        /**
         * @brief Returns a const reference to the logger instance for the specified scope.
         *
         * Will fall back to the default logger if the scope was not registered yet.
         * This method is thread safe for map look-ups.
         *
         * @param scope The textual scope name of the requested logger.
         *
         * @return The logger registered with the scope, or the default logger if
         * none was found.
         */
        [[nodiscard]] const Logger& logger(const std::string& scope) const noexcept {
            // mapMutex_ is automatically released when going out of scope
            std::lock_guard<std::mutex> lock(mapMutex_);

            auto log = loggers_.find(scope);
            if (log != loggers_.end()) {
                return *(log->second);
            }

            return *defaultLogger_;
        }

        /**
         * @brief Registers a new logger with this manager.
         *
         * This method is thread safe for map modifications.
         *
         * @param scope The scope requested for the logger to create.
         *
         * @return The logger registered with the scope, or the logger already registered
         * with the LogManager under the given scope.
         */
        [[nodiscard]] Logger& registerLogger(const std::string& scope) noexcept {
            // mapMutex_ is automatically released when going out of scope
            std::lock_guard<std::mutex> lock(mapMutex_);

            if (auto log = loggers_.find(scope); log != loggers_.end()) {
                return *(log->second);
            }

            auto logger = std::make_unique<Logger>(scope);
            loggers_[scope] = std::move(logger);
            loggers_[scope]->enable(loggingEnabled_);

            // Configure sinks for new logger
            {
                std::lock_guard<std::mutex> sinkLock(sinkMutex_);
                for (const auto& sink : registeredSinks_) {
                    if (sink && enabledSinks_.contains(sink->typeId())) {
                        loggers_[scope]->addSink(sink);
                    }
                }
            }

            return *loggers_[scope];
        }

        /**
         * @brief Enables or disables all log output of the Loggers registered with this LogManager.
         *
         * @param enable True to enable log output with the registered loggers, otherwise false.
         */
        void enableLogging(bool enable) noexcept {
            std::lock_guard<std::mutex> lock(mapMutex_);

            if (loggingEnabled_ == enable) {
                return;
            }

            loggingEnabled_ = enable;

            for (auto& [fst, snd]: loggers_) {
                snd->enable(enable);
            }
        }

        /**
         * @brief Sets the filter scope for the logger.
         *
         * Will do nothing if logging is not enabled. If the logger for the scope does not
         * exist, it will get implicitly created.
         *
         * @param scope The scope to filter. Only log messages with this scope will be logged.
         */
        void setScopeFilter(const std::string& scope) noexcept {
            if (!loggingEnabled_) {
                return;
            }

            // Make sure the logger exists first (this acquires mapMutex_ internally)
            std::ignore = LogManager::getInstance().registerLogger(scope);

            // Now lock and update the filter
            std::lock_guard<std::mutex> lock(mapMutex_);

            for (auto& [fst, snd] : loggers_) {
                if (fst == scope) {
                    snd->enable(true);
                } else {
                    snd->enable(false);
                }
            }
        }

        // ===== Sink Management =====

        /**
         * @brief Registers a sink and enables it by default.
         *
         * The sink is added to the pool and immediately enabled for output.
         *
         * @param sink The sink to register.
         */
        void registerSink(std::shared_ptr<LogSink> sink) {
            registerSink(std::move(sink), true);
        }

        /**
         * @brief Registers a sink with optional auto-enable.
         *
         * @param sink The sink to register.
         * @param enabled Whether to enable the sink immediately (default: true).
         */
        void registerSink(std::shared_ptr<LogSink> sink, bool enabled) {
            if (!sink) return;

            std::lock_guard<std::mutex> lock(sinkMutex_);

            // Check if sink with same typeId is already registered
            bool alreadyRegistered = false;
            for (const auto& existing : registeredSinks_) {
                if (existing && std::strcmp(existing->typeId(), sink->typeId()) == 0) {
                    alreadyRegistered = true;
                    break;
                }
            }

            if (!alreadyRegistered) {
                registeredSinks_.push_back(sink);
            }

            if (enabled) {
                enabledSinks_.insert(sink->typeId());
            }

            updateLoggerSinks();
        }

        /**
         * @brief Enables a sink by its type identifier.
         *
         * If a sink with the given typeId is registered, it will be enabled.
         * If no sink with that typeId is registered, this call has no effect.
         *
         * @param typeId The unique type identifier of the sink (e.g., "console", "imgui").
         */
        void enableSink(SinkTypeId typeId) {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            enabledSinks_.insert(typeId);
            updateLoggerSinks();
        }

        /**
         * @brief Enables a sink, registering it first if necessary.
         *
         * If the sink is not yet registered, it will be added to the pool.
         * The sink is then enabled for output.
         *
         * @param sink The sink instance to enable (and register if needed).
         */
        void enableSink(std::shared_ptr<LogSink> sink) {
            if (!sink) return;

            std::lock_guard<std::mutex> lock(sinkMutex_);

            // Check if already registered
            bool alreadyRegistered = false;
            for (const auto& existing : registeredSinks_) {
                if (existing && std::strcmp(existing->typeId(), sink->typeId()) == 0) {
                    alreadyRegistered = true;
                    break;
                }
            }

            // Auto-register if not already registered
            if (!alreadyRegistered) {
                registeredSinks_.push_back(sink);
            }

            enabledSinks_.insert(sink->typeId());
            updateLoggerSinks();
        }

        /**
         * @brief Disables a sink by its type identifier.
         *
         * @param typeId The unique type identifier of the sink.
         */
        void disableSink(SinkTypeId typeId) {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            enabledSinks_.erase(typeId);
            updateLoggerSinks();
        }

        /**
         * @brief Disables a sink by instance.
         *
         * @param sink The sink instance to disable.
         */
        void disableSink(std::shared_ptr<LogSink> sink) {
            if (!sink) return;
            disableSink(sink->typeId());
        }

        /**
         * @brief Checks if a sink with the given type identifier is currently enabled.
         *
         * @param typeId The unique type identifier of the sink.
         *
         * @return True if the sink is enabled.
         */
        [[nodiscard]] bool isSinkEnabled(SinkTypeId typeId) const noexcept {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            return enabledSinks_.contains(typeId);
        }

        /**
         * @brief Enables all registered sinks.
         */
        void enableAllSinks() {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            for (const auto& sink : registeredSinks_) {
                if (sink) {
                    enabledSinks_.insert(sink->typeId());
                }
            }
            updateLoggerSinks();
        }

        /**
         * @brief Disables all sinks.
         */
        void disableAllSinks() {
            std::lock_guard<std::mutex> lock(sinkMutex_);
            enabledSinks_.clear();
            updateLoggerSinks();
        }
    };

}
