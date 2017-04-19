#ifndef TIMECODE_H_INCLUDE
#define TIMECODE_H_INCLUDE

#include <string>
#include <sstream>
#include <ctime>

class TimeCode
{
public:
  /*
  static const std::string MONTHS[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
  };
  */
  TimeCode() : years(1970), months(0), days(1), hours(0), minutes(0), seconds(0) {};
  TimeCode(const std::string& str);
  //TimeCode(const std::string&& str);

  TimeCode(const TimeCode& copy)
    :years(copy.years),months(copy.months),days(copy.days)
    ,hours(copy.hours),minutes(copy.minutes),seconds(copy.seconds)
  {}
  TimeCode(const TimeCode&& move)
    :years(move.years),months(move.months),days(move.days)
    ,hours(move.hours),minutes(move.minutes),seconds(move.seconds)
  {}

  inline time_t getYear() const { return years; }
  inline time_t getMonth() const { return months; }
  inline time_t getDay() const { return days; }
  inline time_t getHours() const { return hours; }
  inline time_t getMinutes() const { return minutes; }
  inline time_t getSeconds() const { return seconds; }

  inline bool operator==(const TimeCode& other) const { return static_cast<std::time_t>(*this) == other; }
  inline bool operator!=(const TimeCode& other) const { return !(*this == other); }
  inline bool operator<(const TimeCode& other) const { return static_cast<std::time_t>(*this) < other; }
  inline bool operator>(const TimeCode& other) const { return static_cast<std::time_t>(*this) > other; }
  inline bool operator<=(const TimeCode& other) const { return !(*this < other); }
  inline bool operator>=(const TimeCode& other) const { return !(*this > other); }

  operator std::time_t() const;

  friend std::ostream& operator<<(std::ostream& os, const TimeCode& tc);
  friend std::istream& operator>>(std::istream& is, TimeCode& tc);
private:
  std::time_t years;
  std::time_t months;
  std::time_t days;
  std::time_t hours;
  std::time_t minutes;
  std::time_t seconds;
};

#endif
