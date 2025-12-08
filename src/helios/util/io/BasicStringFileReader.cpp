module;

#include <format>
#include <fstream>
#include <string>

module helios.util.io.BasicStringFileReader;

namespace helios::util::io {

    std::string BasicStringFileReader::getContents(const std::string& filename) const {
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

    bool BasicStringFileReader::readInto( const std::string& filename,  std::string& contents) const noexcept {
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


}