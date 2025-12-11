#ifndef CHRONOMETER_H
#define CHRONOMETER_H

#include <chrono>

class Chronometer {
    public:
        Chronometer(bool start_now);
        ~Chronometer() = default;

        void start();
        void restart();

        void stop();
        void pause();

        std::uint64_t getElapsedMs();
        bool isStarted() { return started; }

    private:
    std::chrono::steady_clock::time_point t0;
    std::uint64_t elapsedBeforePauseMS;
    bool started;
};

#endif