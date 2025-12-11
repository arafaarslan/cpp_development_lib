#include "Timer.h"

Timer::Timer() 
    : tmrPtr(nullptr)
    , callback_(nullptr)
    , firstStartTimeMS(0)
    , lastStartTimeMS(0)
    , lastElapsedTimeMS(0)
    , alarmKind(Kind::kOneShot) {

} 

Timer::Timer(asio::io_context& io_ctx) : Timer() {
    tmrPtr = new asio::steady_timer(io_ctx);
} 

Timer::~Timer() {
    delete tmrPtr;
}

void Timer::setContext(asio::io_context& io_ctx) {
    if (tmrPtr != nullptr){
        stop();
        delete tmrPtr;
    }
    tmrPtr= new asio::steady_timer(io_ctx);
}

void Timer::set(std::uint32_t duration_ms, std::function<void(void)> callback) {
    set(Kind::kOneShot, duration_ms, callback);
}

void Timer::set(Kind alarm_kind, std::uint32_t duration_ms, std::function<void(void)> callback) {
    alarmKind = alarm_kind;
    durationMS = duration_ms;
    callback_ = callback;
    skipCallbackLimitMS = duration_ms / kAlarmDurToSkipDurRatio;
}

void Timer::start() {
    lastElapsedTimeMS = 0;
    firstStartTimeMS = Watch::timeInMs(Watch::Kind::Steady);
    start(durationMS);
}

void Timer::start(std::uint32_t time_to_next_alarm_ms) {
    
    if (tmrPtr != nullptr) {
        tmrPtr->expires_after(asio::chrono::milliseconds(time_to_next_alarm_ms));
        
        tmrPtr->async_wait([this](asio::error_code error_code) {
            if (error_code != asio::error::operation_aborted) {
                this->alarm();
            }
        });
    }
    lastStartTimeMS = Watch::timeInMs(Watch::Kind::Steady);
}

void Timer::alarm() {
    // Record the alarm time to indicate alarm has been expired
    lastElapsedTimeMS = Watch::timeInMs(Watch::Kind::Steady);

    callback_();

    if (alarmKind != Kind::kOneShot) {
        uint32_t time_to_next_alarm_ms = calculateTimeToNextAlarm();
        start(time_to_next_alarm_ms);
    }
}

void Timer::stop() {
    if (tmrPtr != nullptr) {
        tmrPtr->cancel();
    }

    firstStartTimeMS = 0;
    lastStartTimeMS = 0;
    lastElapsedTimeMS = 0;
}

uint32_t Timer::calculateTimeToNextAlarm() {
    std::uint64_t now_ms = Watch::timeInMs(Watch::Kind::Steady);
    uint32_t time_to_next_alarm_ms = 0;
   
    if (alarmKind == Kind::kPeriodic) {
        std::uint64_t ms_after_first_set = now_ms - firstStartTimeMS;
        // To calculate next peroid point, this value needs to be normalized with durationMS
        std::uint64_t duration_spent_in_moving_window =  (ms_after_first_set) % durationMS;
        time_to_next_alarm_ms = durationMS - duration_spent_in_moving_window;
        
        // If upcoming alarm time is too close, it is also dropped
        // to prevent hogging the system
        if (time_to_next_alarm_ms < skipCallbackLimitMS) {
            time_to_next_alarm_ms = durationMS;
        }
      
    } else if (alarmKind == Kind::kAutoRestart) {
        time_to_next_alarm_ms = durationMS;
    }

    return time_to_next_alarm_ms;
}

Timer::State Timer::checkAndClearState(){
    State state = State::kIdle;
    
    // If elapsed time has a non-zero value, that means timer elapsed at least once
    if (lastElapsedTimeMS != 0) {
        state = State::kElapsed;
        if (lastStartTimeMS < lastElapsedTimeMS) {
            // Alarm is one-shot alarm and not manually restarted after elapse
            lastStartTimeMS = 0;
        }
        lastElapsedTimeMS = 0;
        // Else: Alarm is manually/automaticaly restarted
        //  Do not touch lastStartTimeMS to 
        //  avoid problems for auto-restart/periodic timers            
    
    // If start time has a non-zero value, that means timer has been started 
    } else if (lastStartTimeMS != 0) {
        state = State::kRunning;
    
    // If both start time and elapsed time are zero, that indicates timer is idle
    } else {
        state = State::kIdle;
    }

    return state; 
}
