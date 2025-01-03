#include "_7683.h"
#include <sys/stat.h>

Watchdog::Watchdog(const std::string& filename) : filename(filename) {
    struct stat result;
    if (stat(filename.c_str(), &result) == 0) {
        lastModified = result.st_mtime;
    }
}

void Watchdog::check() {
    //outdated
}
