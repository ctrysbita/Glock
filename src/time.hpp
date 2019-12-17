#include <date.h>

#include <chrono>

class Time {
 private:
  Time() {}

  static auto GetTimeOfDay() {
    using namespace date;

    auto time = std::chrono::system_clock::now();
    auto day_point = floor<days>(time);
    auto ymd = year_month_day(day_point);
    return make_time(time - day_point);
  }

 public:
  static auto Hours() { return GetTimeOfDay().hours().count(); }

  static auto Minutes() { return GetTimeOfDay().minutes().count(); }

  static auto Seconds() { return GetTimeOfDay().seconds().count(); }

  static auto Subseconds() { return GetTimeOfDay().subseconds().count(); }

  static auto Milliseconds() {
#ifdef _MSC_VER
    return Subseconds() / 10000;
#elif defined(__GNUC__) && defined(__MINGW32__)
    return Subseconds() / 1000000;
#elif defined(__APPLE__)
    return Subseconds() / 1000;
#else
    return Subseconds();
#endif
  }
};