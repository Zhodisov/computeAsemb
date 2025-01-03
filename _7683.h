#ifndef _7683_H
#define _7683_H

#include <string>
#include <ctime>

class Watchdog {
public:
    Watchdog(const std::string& filename);
    void check();

private:
    std::string filename;
    std::time_t lastModified;
};

#endif 