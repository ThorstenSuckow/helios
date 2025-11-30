module;

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

module helios.util.log.Logger;

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
        return *loggers_[scope];
    }
};



