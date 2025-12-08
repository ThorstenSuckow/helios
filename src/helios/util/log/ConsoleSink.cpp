module;

#include <iostream>
#include <string>

module helios.util.log.ConsoleSink;

import helios.util.log.LogSink;

namespace helios::util::log {

    SinkTypeId ConsoleSink::typeId() const noexcept {
        return TYPE_ID;
    }

    void ConsoleSink::write(LogLevel level, const std::string& scope, const std::string& message) {
        const char* levelStr = "";
        switch (level) {
            case LogLevel::Debug: levelStr = "[DEBUG]"; break;
            case LogLevel::Info:  levelStr = "[INFO]";  break;
            case LogLevel::Warn:  levelStr = "[WARN]";  break;
            case LogLevel::Error: levelStr = "[ERROR]"; break;
        }
        std::cout << levelStr << "[" << scope << "] " << message << std::endl;
    }

    void ConsoleSink::flush() {
        std::cout.flush();
    }

} // namespace helios::util::log

