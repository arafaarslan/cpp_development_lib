#include "Logger.h"

std::string Logger::tag_ = "AS";
Logger::Level Logger::level_ = Logger::Level::kError;
std::mutex Logger::log_mutex_;
