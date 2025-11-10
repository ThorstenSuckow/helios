/**
 * @brief Abstract base class for reading file contents into a string.
 */
module;

#include <string>

export module helios.util.io.StringFileReader;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::util::io.FileReader"
export namespace helios::util::io {


    /**
     * @brief Abstract base class for reading file contents into a string.
     *
     */
    class StringFileReader {

    protected:
        /**
         * @brief The logger used with this FileReader.
         */
        helios::util::log::Logger logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);


    public:
        virtual ~StringFileReader() = default;

        /**
         * Default constructor.
         */
        StringFileReader() = default;

        /**
         * @brief Reads the entire contents of the file and returns them as a string.
         *
         * @param filename The path to the file to read.
         * @return The contents of the file as a std::string. If reading fails, an empty string is returned.
         *
         * @throws std::runtime_error if the file operation fails.
         */
        [[nodiscard]] virtual std::string getContents(const std::string& filename) const  = 0;

        /**
         * @brief Attempts to read the entire contents of the file into the provided string.
         *
         * @param filename The path to the file to read.
         * @param contents Output parameter receiving the file contents on success.
         * @return true if reading succeeded and `contents` was populated, otherwise false.
         */
        [[nodiscard]] virtual bool readInto( const std::string& filename,  std::string& contents) const noexcept = 0;


    };


}