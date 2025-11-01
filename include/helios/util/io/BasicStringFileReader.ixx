module;

#include <string>

export module helios.util.io.BasicStringFileReader;

import helios.util.io.StringFileReader;

export namespace helios::util::io {


    /**
     * @brief Basic implementation of a string file reader.
     */
    class BasicStringFileReader : public StringFileReader {


    public:
        std::string getContents(const std::string& filename) const  override;

        bool readInto( const std::string& filename,  std::string& contents) const noexcept override;
    };


}