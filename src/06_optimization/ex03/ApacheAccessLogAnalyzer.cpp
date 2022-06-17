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
    //std::cout << "Open" << std::endl;
    myInFile = fopen(myFilename.c_str(), "r");
    if( myInFile == NULL ) {
        std::cout << "Error" << std::endl;
        exit(1);
    }
}

void ApacheAccessLogAnalyzer::closeFile()
{
    fclose(myInFile);
}

void ApacheAccessLogAnalyzer::processFile()
{
    std::ios::sync_with_stdio(false);
    char line[300];
    std::string linecopy;
    std::cout << "Processing log file " << myFilename << std::endl;
    while((fgets(line, 300, myInFile)))
    {
        linecopy = line;
        //std::cout << (linecopy.substr(0, linecopy.find_first_of(" "))) << std::endl;
        //myHostCounter.notifyHost(line.substr(0, line.find_first_of(" ")));
        myHostCounter.myHosts.insert((linecopy.substr(0, linecopy.find_first_of(" ")))); //INLINING
    }
    std::cout << "Found " << myHostCounter.getNbOfHosts() << " unique Hosts/IPs" << std::endl;
}
