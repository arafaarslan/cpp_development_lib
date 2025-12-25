#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <string>
#include <cstring>
#include <iostream>
#include <time.h>
#include <mutex>

//#define START_TIME_FROM_1970 

class Logger {
    public:
        enum Level {kFatal = 0, kError = 1, kHighlight = 2, kInfo = 3, kDebug = 4, kTrace = 5};

        static void SetLogLevel(const char* level) {
            if (strcmp(level, "trace") == 0) {
                Logger::SetLogLevel(Logger::Level::kTrace);
            } else if (strcmp(level, "debug") == 0) {
                Logger::SetLogLevel(Logger::Level::kDebug);
            } else if (strcmp(level, "info") == 0) {
                Logger::SetLogLevel(Logger::Level::kInfo);
            } else if (strcmp(level, "highlight") == 0) {
                Logger::SetLogLevel(Logger::Level::kHighlight);
            } else if (strcmp(level, "error") == 0) {
                Logger::SetLogLevel(Logger::Level::kError);
            } else if (strcmp(level, "fatal") == 0) {
                Logger::SetLogLevel(Logger::Level::kFatal);
            }
        }

        static void SetLogLevel(Level level) {
            level_ = level;
        }

        static void SetTag(std::string tag) {
            tag_ = tag;
        }

        static void GetTime(char* time_str, std::int32_t len) {
            #ifdef START_TIME_FROM_1970
                struct timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                sprintf(time_str, "%5d.%03d", (std::int32_t)ts.tv_sec, (std::int32_t)(ts.tv_nsec / 1000000));            
            #else
                static struct timespec start_ts = {0, 0};
                struct timespec now_ts;
            
                // Initialize start time once
                if (start_ts.tv_sec == 0 && start_ts.tv_nsec == 0) {
                    clock_gettime(CLOCK_MONOTONIC, &start_ts);
                }
            
                clock_gettime(CLOCK_MONOTONIC, &now_ts);
            
                // Compute difference (elapsed time)
                double elapsed = (now_ts.tv_sec - start_ts.tv_sec)
                            + (now_ts.tv_nsec - start_ts.tv_nsec) / 1e9;
            
                snprintf(time_str, len, "%.3f", elapsed);
            #endif
        }

        static void Log(Level level, std::string s) {
            if (level == Level::kFatal) {
                Fatal(s);
            } else if (level == Level::kError) {
                Error(s);
            } else if (level == Level::kHighlight) {
                Highlight(s);
            } else if (level == Level::kInfo) {
                Info(s);
            } else if (level == Level::kDebug) {
                Debug(s);
            } else if (level == Level::kTrace) {
                Trace(s);
            }
        }

        static void Trace(std::string s) {
            char time_str[100];
            GetTime(&time_str[0], sizeof(time_str));
            
            if (Logger::level_ >= Level::kTrace) {
                std::cout <<  Logger::tag_ << "-T[" << std::string(time_str) << "]: "<< s << std::endl;
            }
        }

        static void Debug(std::string s) {
            if (Logger::level_ >= Level::kDebug) {
                char time_str[100];
                GetTime(&time_str[0], sizeof(time_str));
                std::lock_guard<std::mutex> lock(log_mutex_);
                std::cout <<  Logger::tag_ << "-D[" << time_str << "]: "<< s << std::endl;
            }
        }

        static void Info(std::string s) {
            if (Logger::level_ >= Level::kInfo) {
                char time_str[100];
                GetTime(&time_str[0], sizeof(time_str));
                std::lock_guard<std::mutex> lock(log_mutex_);
                std::cout << Logger::tag_ << "-I[" << time_str << "]: "<< s << std::endl;
            }
        }

        static void Highlight(std::string s) {
            if (Logger::level_ >= Level::kHighlight) {
                char time_str[100];
                GetTime(&time_str[0], sizeof(time_str));
                std::lock_guard<std::mutex> lock(log_mutex_);
                std::cout << Logger::tag_ << "-H[" << time_str << "]: "<< s << std::endl;
            }
        }

        static void Error(std::string s){
            if (Logger::level_ >= Level::kError) {
                char time_str[100];
                GetTime(&time_str[0], sizeof(time_str));
                std::lock_guard<std::mutex> lock(log_mutex_);
                std::cout  << ">>>>>>>>" << std::endl << "        " << Logger::tag_ << "-E[" << time_str << "]: "<< s << std::endl << "<<<<<<<<" << std::endl;
            }
        }

        static void Fatal(std::string s){
            if (Logger::level_ >= Level::kFatal) {
                char time_str[100];
                GetTime(&time_str[0], sizeof(time_str));
                std::cout  << ">>>>>>>>" << std::endl << "        " << Logger::tag_ << "-F[" << time_str << "]: "<< s << std::endl << "<<<<<<<<" << std::endl;
            }
        }
        
        static std::string tag_;
        static Level level_;
        static std::mutex log_mutex_;
};

#define TRACE_LOG(message) Logger::Trace(message);

class TraceFunctionLogger {
public:
    TraceFunctionLogger(const char* file, const char* function) : m_file(file), m_function(function) {
        TRACE_LOG(std::string("Entering ") + m_file + std::string("-") + m_function);
    }
    ~TraceFunctionLogger() {
        TRACE_LOG(std::string("Exiting ") + m_file + std::string("-") + m_function);        
    } 
private: 
    const char* m_file;
    const char* m_function;
};

#define TRACE_FUNCTION() TraceFunctionLogger logger(__FILE__, __PRETTY_FUNCTION__)
#define LINE_TRACE() TRACE_LOG(__PRETTY_FUNCTION__ + std::string("-") + std::to_string(__LINE__));


#endif // LOGGER_H