module;

#include <iostream>
#include <string>
#include <mutex>

module helios.util.log.Logger;

import helios.util.log.LogSink;

namespace helios::util::log {

    Logger::Logger(std::string scope) :
        scope_(std::move(scope)) {}

    void Logger::dispatch(LogLevel level, const std::string& msg) const noexcept {
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

    void Logger::enable(const bool enable) noexcept {
        enabled_ = enable;
    }

    void Logger::addSink(std::shared_ptr<LogSink> sink) {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        sinks_.push_back(std::move(sink));
    }

    void Logger::clearSinks() noexcept {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        sinks_.clear();
    }

    size_t Logger::sinkCount() const noexcept {
        std::lock_guard<std::mutex> lock(sinkMutex_);
        return sinks_.size();
    }

    void Logger::warn(const std::string& msg) const noexcept {
        dispatch(LogLevel::Warn, msg);
    }

    void Logger::debug(const std::string& msg) const noexcept {
        dispatch(LogLevel::Debug, msg);
    }

    void Logger::info(const std::string& msg) const noexcept {
        dispatch(LogLevel::Info, msg);
    }

    void Logger::error(const std::string& msg) const noexcept {
        dispatch(LogLevel::Error, msg);
    }

} // namespace helios::util::log

