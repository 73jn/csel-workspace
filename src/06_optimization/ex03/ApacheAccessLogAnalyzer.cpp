#include "ApacheAccessLogAnalyzer.h"

#include <iostream>
#include <sstream>
#include <algorithm>


ApacheAccessLogAnalyzer::ApacheAccessLogAnalyzer(std::string filename)
    : myFilename(filename)
{

}

void ApacheAccessLogAnalyzer::openFile()
{
    myInFile = fopen(myFilename.c_str(), "r");
}

void ApacheAccessLogAnalyzer::closeFile()
{
    fclose(myInFile);
}

void ApacheAccessLogAnalyzer::processFile()
{
    std::ios::sync_with_stdio(false);
    char* line;
    std::string linecopy;
    std::cout << "Processing log file " << myFilename << std::endl;
    while((fgets(line, 100, myInFile)))
    {
        linecopy = line;
        //myHostCounter.notifyHost(line.substr(0, line.find_first_of(" ")));
        myHostCounter.myHosts.insert((linecopy.substr(0, linecopy.find_first_of(" ")))); //INLINING
    }
    std::cout << "Found " << myHostCounter.getNbOfHosts() << " unique Hosts/IPs" << std::endl;
}
