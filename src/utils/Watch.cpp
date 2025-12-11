#include "Watch.h"


Watch::Watch() {

}

uint64_t Watch::timeInMs(Kind kind) {
  uint64_t currentTimeMs = 0;

  if (kind == Kind::Steady) {
    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    currentTimeMs = ms.time_since_epoch().count();
  } else if (kind == Kind::Utc) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    currentTimeMs = ms.time_since_epoch().count();
  } else if (kind == Kind::Local) {

  }

  return currentTimeMs;
}
