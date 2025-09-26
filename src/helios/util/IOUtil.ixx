module;

#include <iostream>
#include <fstream>
#include <string>

export module helios.util:IOUtil;


export namespace helios::util {

    class IOUtil {

    public:
        static std::string getContents( std::string filename) {

            std::ifstream infile {filename};

            if (!infile) {
                std::cerr << "Cannot open " << filename <<  std::endl;
            }

            std::string line;
            std::string fileContents = "";
            while (getline(infile, line)) {
                fileContents += line + "\n";
            }

            return fileContents;
        }

        static void readInto( std::string filename,  std::string& contents) {

            std::ifstream infile{ filename };

            if (!infile) {
                std::cerr << "Cannot open " << filename <<  std::endl;
                return;
            }

            std::string line;
            std::string fileContents = "";

            while (getline(infile, line)) {
                fileContents += line + "\n";
            }

            contents = fileContents;
        }



    };



}