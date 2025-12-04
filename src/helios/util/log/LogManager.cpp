module;

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <vector>
#include <cstring>

module helios.util.log.Logger;

import helios.util.log.LogSink;
import helios.util.log.LogManager;


namespace helios::util::log {


    LogManager::LogManager() : defaultLogger_(std::make_unique<Logger>("default")) {}

    const Logger& LogManager::loggerForScope(const std::string& scope) noexcept {
        return LogManager::getInstance().registerLogger(scope);
    }


    LogManager& LogManager::getInstance() noexcept {
        static LogManager instance;

        return instance;
    }


    [[nodiscard]] const Logger& LogManager::logger() const noexcept {
        return *defaultLogger_;
    }

    void LogManager::setScopeFilter(const std::string& scope) noexcept {
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

    void LogManager::enableLogging(const bool enable) noexcept {
        std::lock_guard<std::mutex> lock(mapMutex_);

        if (loggingEnabled_ == enable) {
            return;
        }

        loggingEnabled_ = enable;

        for (auto& [fst, snd]: loggers_) {
            snd->enable(enable);
        }

    }


    [[nodiscard]] const Logger& LogManager::logger(const std::string& scope) const noexcept {
        // mapMutex_ is automatically released when going out of scope
        std::lock_guard<std::mutex> lock(mapMutex_);

        auto log = loggers_.find(scope);
        if (log != loggers_.end()) {
            return *(log->second);
        }

        return *defaultLogger_;
    }


    [[nodiscard]] Logger& LogManager::registerLogger(const std::string& scope) noexcept {
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

    // ===== Sink Management Implementation =====

    void LogManager::updateLoggerSinks() {
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

    void LogManager::registerSink(std::shared_ptr<LogSink> sink) {
        registerSink(std::move(sink), true);
    }

    void LogManager::registerSink(std::shared_ptr<LogSink> sink, bool enabled) {
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

    void LogManager::enableSink(SinkTypeId typeId) {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        enabledSinks_.insert(typeId);
        updateLoggerSinks();
    }

    void LogManager::enableSink(std::shared_ptr<LogSink> sink) {
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

    void LogManager::disableSink(SinkTypeId typeId) {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        enabledSinks_.erase(typeId);
        updateLoggerSinks();
    }

    void LogManager::disableSink(std::shared_ptr<LogSink> sink) {
        if (!sink) return;
        disableSink(sink->typeId());
    }

    [[nodiscard]] bool LogManager::isSinkEnabled(SinkTypeId typeId) const noexcept {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        return enabledSinks_.contains(typeId);
    }

    void LogManager::enableAllSinks() {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        for (const auto& sink : registeredSinks_) {
            if (sink) {
                enabledSinks_.insert(sink->typeId());
            }
        }
        updateLoggerSinks();
    }

    void LogManager::disableAllSinks() {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        enabledSinks_.clear();
        updateLoggerSinks();
    }

};



