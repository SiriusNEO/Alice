/*!
 * \file src/utils/logging.hpp
 * \brief Logging utils. Modified from:
 *
 *      https://github.com/SiriusNEO/CoconutJVM/blob/main/src/utils/logging.h.
 *
 * \author SiriusNEO
 */

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#ifndef UTILS_LOGGING_HPP_
#define UTILS_LOGGING_HPP_

namespace alice {

namespace utils {

constexpr const char* kALICE_IERROR_MSG =
    "\n"
    "---------------------------------------------------------------\n"
    "Interal error happens in ALICE. \n"
    "Please check the source code for detailed reason. \n"
    "---------------------------------------------------------------\n";

class AlicePanic : public std::exception {
 public:
  AlicePanic(const std::string& file, int lineno, const std::string& message)
      : message_(kALICE_IERROR_MSG + file + ":" + std::to_string(lineno) +
                 ": " + message) {}

  virtual const char* what() const noexcept { return message_.c_str(); }

 private:
  std::string message_;
};

#define kALICE_LOG_INFO "INFO"
#define kALICE_LOG_DEBUG "DEBUG"
#define kALICE_LOG_WARNING "WARNING"
#define kALICE_LOG_ERROR "ERROR"
#define kALICE_LOG_FATAL "FATAL"

/*!
 * \brief Class to accumulate an log message.
 * Ref:
 * https://github.com/apache/tvm/blob/HEAD/include/tvm/runtime/logging.h#L323-L339
 */
class LogMessage {
 public:
  LogMessage(const std::string& file, int lineno, const std::string& level)
      : file_(file), lineno_(lineno) {
    static auto LOG_LEVEL_LIST = {"INFO", "DEBUG", "WARNING", "ERROR", "FATAL"};

    for (const auto& valid_level : LOG_LEVEL_LIST) {
      if (valid_level == level) {
        level_ = valid_level;
        break;
      }
    }
    if (level_.empty()) {
      throw AlicePanic(file, lineno, "Invalid Log Level: " + level);
    }
  }

  ~LogMessage() noexcept(false) {
    std::time_t time = std::time(nullptr);
    std::ostringstream info;
    info << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] "
         << file_ << ":" << lineno_ << ": "
         << "["
         << "ALICE LOG " << level_ << "] " << stream_.str() << std::endl;
    if (level_ == kALICE_LOG_FATAL) {
      // std::cerr << info.str();
      throw AlicePanic(file_, lineno_, stream_.str());
    } else {
      // colored log
      std::cerr << "\033[34m" << info.str() << "\033[0m";
    }
  }

  std::ostringstream& stream() { return stream_; }

 private:
  std::string file_;
  int lineno_;
  std::string level_;
  std::ostringstream stream_;
};

#define LOG(level) LOG_##level
#define LOG_DEBUG \
  utils::LogMessage(__FILE__, __LINE__, kALICE_LOG_DEBUG).stream()
#define LOG_INFO utils::LogMessage(__FILE__, __LINE__, kALICE_LOG_INFO).stream()
#define LOG_ERROR \
  utils::LogMessage(__FILE__, __LINE__, kALICE_LOG_ERROR).stream()
#define LOG_WARNING \
  utils::LogMessage(__FILE__, __LINE__, kALICE_LOG_WARNING).stream()
#define LOG_FATAL \
  utils::LogMessage(__FILE__, __LINE__, kALICE_LOG_FATAL).stream()

#define CHECK(x)                                                   \
  if (!(x))                                                        \
  utils::LogMessage(__FILE__, __LINE__, kALICE_LOG_FATAL).stream() \
      << "Check failed: (" #x << ") is false: "

}  // namespace utils

}  // namespace alice

#endif  // UTILS_LOGGING_HPP_
