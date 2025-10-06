module;

#include <string>

export module helios.util.io.StringFileReader;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::util::io.FileReader"
export namespace helios::util::io {


    /**
     * Abstract base class for reading file contents into a string.
     *
     */
    class StringFileReader {

    protected:
        /**
         * The Logegr used with this FileReader.
         */
        helios::util::log::Logger logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);


    public:
        virtual ~StringFileReader() = default;

        /**
         * Default constructor.
         */
        StringFileReader() = default;

        /**
         * Reads the file at the specified path.
         *
         * @param filename The path to the file-object to read from.
         *
         * @return The contents of the file as a string.
         *
         * @throws std::runtime_error
         */
        virtual std::string getContents(const std::string& filename) const  = 0;

        /**
         * Reads the contents of the file into the specified string.
         *
         * @param filename The path to the file-object to read from.
         * @param contents A ref to the string to write the file contents into.
         *
         * @return True if reading succeeded, otherwise false.
         */
        virtual bool readInto( const std::string& filename,  std::string& contents) const noexcept = 0;


    };


}