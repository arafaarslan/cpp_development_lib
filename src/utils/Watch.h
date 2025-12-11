#ifndef WATCH_H_
#define WATCH_H_

#include <cstdint>
#include <chrono>

class Watch {
    public:
      Watch();        
      ~Watch() = default;

      enum Kind {Steady, Utc, Local};
      static uint64_t timeInMs(Kind kind);

};

#endif // WATCH_H_
