#ifndef TIMER_H
#define TIMER_H

#include <iostream>

#include <cstdint>
#include <functional>
#include <asio.hpp>
#include <asio/high_resolution_timer.hpp>

#include "Logger.h"
#include "Watch.h"

class Timer {
    public:
        Timer();
        Timer(asio::io_context& io_ctx);
        ~Timer();

        enum Kind{kOneShot, kAutoRestart, kPeriodic};
        enum State{kIdle, kRunning, kElapsed};

        void setContext(asio::io_context& io_ctx);
        void set(std::uint32_t duration_ms, std::function<void(void)> callback);
        void set(Kind alarm_kind, std::uint32_t duration_ms, std::function<void(void)> callback);
        void start();
        void stop();

        State checkAndClearState();
    private:
        void start(std::uint32_t time_to_next_alarm_ms);
        void alarm();
        
        uint32_t calculateTimeToNextAlarm();

        asio::steady_timer* tmrPtr;
        std::function<void(void)> callback_;
        std::uint32_t durationMS;        
        std::uint32_t skipCallbackLimitMS; 
        Kind alarmKind;

        std::uint64_t firstStartTimeMS;
        std::uint64_t lastStartTimeMS;
        std::uint64_t lastElapsedTimeMS;

        static constexpr uint32_t kAlarmDurToSkipDurRatio = 10;
        
};

#endif // TIMER_H
