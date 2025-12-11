#include "Chronometer.h"

Chronometer::Chronometer(bool start_now): elapsedBeforePauseMS(0), started(false) {
    if (start_now) {
        start();
    }
}

void Chronometer::start() {
    t0 = std::chrono::steady_clock::now();
    started = true;
}

void Chronometer::restart() {
    stop();
    start();
}

void Chronometer::stop() {
    started = 0;
    elapsedBeforePauseMS = 0;
}

void Chronometer::pause() {
    elapsedBeforePauseMS = getElapsedMs(); 
    started = false;
}

std::uint64_t Chronometer::getElapsedMs() {
    std::uint64_t elapsed_ms = elapsedBeforePauseMS;
    if (started == true) {
        auto elapsed = 0;
        elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count();
        elapsed_ms = elapsed_ms + elapsedBeforePauseMS;
    }
    return elapsed_ms;
}