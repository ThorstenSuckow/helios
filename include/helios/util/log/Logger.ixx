module;

#include <iostream>
#include <string>

export module helios.util.log.Logger;

export namespace helios::util::log {

    /**
     * @brief Simple Logger implementation.
     *
     * @todo configure log stream
     */
    class Logger {

    private:
        const std::string scope_ = "default";

        /**
         * @brief flag to indicate whether this Logger's output is enabled.
         */
        bool enabled_ = true;

    public:
        /**
         * @brief Creates a new Logger, tagged with specific scope.
         *
         * @param scope
         */
        explicit Logger(std::string scope) :
            scope_(std::move(scope)) {}


        /**
         * @brief Enables/disables log output for this Logger-instance.
         *
         * @param enable true to enable this log output, false to disable.
         */
        void enable(const bool enable) noexcept {
            enabled_ = enable;
        }

        /**
         * @brief Couts a warning.
         *
         * @param msg
         */
        void inline warn(const std::string& msg) const noexcept{
            enabled_ && std::cout << "[WARN]" << "[" << scope_ << "] " << msg << std::endl;
        }


        /**
         * @brief Couts a msg.
         *
         * @param msg
         */
        void inline debug(const std::string& msg) const noexcept {
            enabled_ && std::cout << "[DEBUG]" << "[" << scope_ << "] " << msg << std::endl;
        }


        /**
         * @brief Couts an info.
         *
         * @param msg
         */
        void inline info(const std::string& msg)  const noexcept{
            enabled_ && std::cout << "[INFO]" << "[" << scope_ << "] " << msg << std::endl;
        }


        /**
         * @brief Couts an error.
         *
         * @param msg
         */
        void inline error(const std::string& msg) const noexcept {
            enabled_ && std::cout << "[ERROR]" << "[" << scope_ << "] " << msg << std::endl;
        }

    };


}