/**
 * @file BasicStringFileReader.ixx
 * @brief Basic file-to-string reader implementation.
 */
module;

#include <format>
#include <fstream>
#include <string>
#include <stdexcept>

export module helios.util.io.BasicStringFileReader;

import helios.util.io.StringFileReader;

export namespace helios::util::io {


    /**
     * @brief Basic implementation of a string file reader.
     */
    class BasicStringFileReader : public StringFileReader {


    public:
        /**
         * @copydoc StringFileReader::getContents()
         */
        [[nodiscard]] std::string getContents(const std::string& filename) const  override {
            std::ifstream infile {filename};

            if (!infile) {
                std::string msg = std::format("Cannot open {0}", filename);
                logger_.error(msg);
                throw std::runtime_error(msg);
            }

            std::string line;
            std::string fileContents;
            while (getline(infile, line)) {
                fileContents += line + "\n";
            }
            return fileContents;
        }

        /**
        * @copydoc StringFileReader::readInto()
         */
        [[nodiscard]] bool readInto( const std::string& filename,  std::string& contents) const noexcept override {
            std::ifstream infile{ filename };

            if (!infile) {
                logger_.error(std::format("Cannot open {0}", filename));
                return false;
            }
            std::string line;
            std::string fileContents;

            while (getline(infile, line)) {
                fileContents += line + "\n";
            }

            contents = fileContents;
            return true;
        }
    };


}