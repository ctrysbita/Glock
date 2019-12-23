#pragma once
#include <date.h>

#include <chrono>

/**
 * @brief Utilities to get accurate system time.
 */
class Time {
 private:
  Time() {}

  /**
   * @brief Get time of day.
   *
   * @return date::hh_mm_ss Time of day.
   */
  static auto GetTimeOfDay() {
    using namespace date;

    auto time = std::chrono::system_clock::now();
    auto day_point = floor<days>(time);
    auto ymd = year_month_day(day_point);
    return make_time(time - day_point);
  }

 public:
  /**
   * @brief Get current hours.
   *
   * @return int64_t Hours of day.
   */
  static auto Hours() { return GetTimeOfDay().hours().count(); }

  /**
   * @brief Get current minutes.
   *
   * @return int64_t Minutes of hour.
   */
  static auto Minutes() { return GetTimeOfDay().minutes().count(); }

  /**
   * @brief Get current seconds.
   *
   * @return int64_t Seconds of minute.
   */
  static auto Seconds() { return GetTimeOfDay().seconds().count(); }

  /**
   * @brief Get current subseconds. Different platform may have different
   * accuracy.
   *
   * @return int64_t Subseconds of second.
   */
  static auto Subseconds() { return GetTimeOfDay().subseconds().count(); }

  /**
   * @brief Get current milliseconds. Automatically converted from subseconds
   * according to platfrom.
   *
   * @return int64_t Milliseconds of second.
   */
  static auto Milliseconds() {
#ifdef _MSC_VER
    return Subseconds() / 10000;
#elif defined(__MINGW32__) || defined(__linux)
    return Subseconds() / 1000000;
#elif defined(__APPLE__)
    return Subseconds() / 1000;
#else
    return Subseconds();
#endif
  }
};