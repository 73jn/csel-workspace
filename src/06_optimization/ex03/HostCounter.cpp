#include "HostCounter.h"

#include <algorithm> // for std::find

HostCounter::HostCounter()
{
}

bool HostCounter::isNewHost(std::string hostname)
{
    return myHosts.count(hostname) != 0;
}

void HostCounter::notifyHost(std::string hostname)
{
    myHosts.insert(move(hostname)); 
}

int HostCounter::getNbOfHosts()
{
    return myHosts.size();
}
