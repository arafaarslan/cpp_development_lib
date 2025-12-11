#include "FileUtils.h"

bool FileUtils::directoryExists(const std::string &path, int timeout_seconds)
{
  DIR* dir = opendir(path.c_str());
  if(dir)
  {
    closedir(dir);
    return true;
  }
  else
  {
    if (timeout_seconds <= 0)
    {
      return false;
    }
    timespec ts = {0, 100000000}; // 100 milliseconds
    nanosleep(&ts, NULL);
    return directoryExists(path, timeout_seconds - 1);
  }
}