#include <chrono>
#include <cmath>
#include <ctime>
#include "date.hpp"

class TimeController {
  std::time_t t;
  std::tm *now;
  std::chrono::time_point<std::chrono::system_clock> tp;
  std::chrono::time_point<std::chrono::system_clock, date::days> dp;
  date::hh_mm_ss<std::chrono::microseconds> now_sec;

 public:
  TimeController()
      : t(std::time(0)),
        now(std::localtime(&t)),
        tp(std::chrono::system_clock::now()),
        now_sec(date::make_time(tp - dp)) {}
  void UpdateTime() {
    t = std::time(0);
    now = std::localtime(&t);
    tp = std::chrono::system_clock::now();
    dp = date::floor<date::days>(tp);
    now_sec = date::make_time(tp - dp);
  }
  int GetHour() { return now->tm_hour; }
  int GetMinute() { return now->tm_min; }
  int GetSec() { return now->tm_sec; }
  long long GetSubsec() { return now_sec.subseconds().count(); }
};