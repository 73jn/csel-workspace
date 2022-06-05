#include "HostCounter.h"

#include <string>
#include <vector>
#include <fstream>

class ApacheAccessLogAnalyzer
{
    public:
        ApacheAccessLogAnalyzer(std::string filename);

        void openFile();
        void closeFile();
        void processFile();

        std::string     myFilename;
        FILE *          myInFile;
        HostCounter     myHostCounter;
};
