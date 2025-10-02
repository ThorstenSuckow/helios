module;

#include <iostream>
#include <string>

export module helios.util.log:Logger;

export namespace helios::util::log {

    /**
     * Simple Logger implementation.
     *
     * @todo configure log stream
     */
    class Logger {

    public:
        void inline warn(const std::string& msg) const noexcept{
            std::cout << "[WARN] " << msg << std::endl;
        }
        void inline debug(const std::string& msg) const noexcept {
            std::cout << "[DEBUG] " << msg << std::endl;
        }
        void inline info(const std::string& msg)  const noexcept{
            std::cout << "[INFO] " << msg << std::endl;
        }
        void inline error(const std::string& msg) const noexcept {
            std::cout << "[ERROR] " << msg << std::endl;
        }

    };


}