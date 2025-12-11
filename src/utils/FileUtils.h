#ifndef _FILE_UTILS_H
#define _FILE_UTILS_H

#include <dirent.h>
#include <unistd.h>
#include <time.h>

#include "Logger.h"

class FileUtils{
    public:
        static bool directoryExists(const std::string &path, int timeout_seconds);
    private:

        
};

#endif