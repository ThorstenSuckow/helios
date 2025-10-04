module;

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

export module helios.util.log.LogManager;

import helios.util.log.Logger;

export namespace helios::util::log {

    /**
     * LogManger for managing various scoped Loggers.
     *
     */
    class LogManager {

    private:

        /**
         * Unordered map holding unique pointers to the loggers managed
         * by this class, guaranteed to be not null.
         */
        std::unordered_map<std::string, std::unique_ptr<Logger>> loggers_;


        /**
         * Default logger if a logger for a specific scope was not
         * found.
         */
        const std::unique_ptr<Logger> defaultLogger_;


        /**
         * Mutex providing mutual exclusive access to the logger_-map.
         */
        mutable std::mutex mapMutex_;


        /**
         * Creates the LogManager and registers an unscoped default logger.
         */
        LogManager();

    public:

        ~LogManager() = default;

        /**
         * Enforce singleton (see meyer's Singleton)
         * @see https://en.wikipedia.org/wiki/Singleton_pattern
         */
        LogManager(const LogManager&) = delete;
        LogManager& operator=(const LogManager&) = delete;


        /**
         * Returns the LogManager singleton instance.
         *
         * @return
         */
        static LogManager& getInstance() noexcept;


        /**
         * Returns a const reference to the default logger managed with this LogManager.
         *
         * @return Logger
         */
        [[nodiscard]] const Logger& logger() const noexcept;


        /**
         * Returns a const reference to the logger instance for the specified scope.
         * Will fall back to the default logger if the scope was not registered yet.
         * This method is thread safe for map look-ups.
         *
         * @param scope The scope of the requested logger.
         *
         * @return The logger registered with the scope, or the default logger if
         * none was found.
         */
        [[nodiscard]] const Logger& logger(const std::string& scope) const noexcept;


        /**
         * Registers a new logger with this manager.
         * This method is thread safe for map modifications.
         *
         * @param scope The scope requested for the logger to create.
         *
         * @return The logger registered with the scope, or the logger already registered
         * with the LogManager under the given scope.
         */
        [[nodiscard]] Logger& registerLogger(const std::string& scope) noexcept;

    };



}