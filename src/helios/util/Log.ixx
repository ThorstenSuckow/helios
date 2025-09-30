module;

#include <iostream>
#include <string>

export module helios.util.Log;

export namespace helios::util {

    class Log {

    public:
        static void warn(const std::string& msg) {
            std::cout << "[WARN] " << msg << std::endl;
        }
        static void debug(const std::string& msg) {
            std::cout << "[DEBUG] " << msg << std::endl;
        }
        static void info(const std::string& msg) {
            std::cout << "[INFO] " << msg << std::endl;
        }
        static void error(const std::string& msg) {
            std::cout << "[ERROR] " << msg << std::endl;
        }

    };


}