#include "TimeCode.h"

#include <chrono>
#include <iomanip> // std::setw, std::setfill
#include <utility> // std::move

// str = dd/mm/yyyyThh:mm:ssz
TimeCode::TimeCode(const std::string& str)
{
  std::string::size_type pos = 0;
  std::string::size_type end = str.find('/');
  days = std::stol(str.substr(pos, end-pos));
  pos = end+1;
  end = str.find('/', pos);
  months = std::stol(str.substr(pos, end-pos))-1;
  pos = end+1;
  end = str.find('T', pos);
  years = std::stol(str.substr(pos, end-pos))-1900;
  pos = end+1;
  end = str.find(':', pos);
  hours = std::stol(str.substr(pos, end-pos));
  pos = end+1;
  end = str.find(':', pos);
  minutes = std::stol(str.substr(pos, end-pos));
  pos = end+1;
  end = str.find('Z', pos);
  seconds = std::stol(str.substr(pos, end-pos));
}

std::time_t leapDays(const std::time_t& year)
{
  return
    (year)/4 // leap year every 4 years
  - (year)/100 // but not every 100, so take away those
  + (year)/400; // but it is every 400, so add those back in
}

TimeCode::operator std::time_t() const
{
  static const std::time_t SECONDS_A_DAY = 86400;
  static const std::time_t SECONDS_A_YEAR = SECONDS_A_DAY * 365;
  static const std::time_t EPOCH = 1970;
  // number of leap days up to 1970-01-01
  static const std::time_t EPOCH_LEAPS = leapDays(EPOCH-1);
  static const std::time_t MONTHS[] = {
    31, // jan
    28, // feb
    31, // mar
    30, // apr
    31, // may
    30, // jun
    31, // jul
    31, // aug
    30, // sep
    31, // oct
    30, // nov
    31
  };

  std::time_t y = years - EPOCH;

  // seconds for each year since EPOCH
  std::time_t ret = y * SECONDS_A_YEAR;

  // seconds for each day of each month of this year
  for (std::time_t i = 0; i < months-1; ++i)
    ret += MONTHS[i] * SECONDS_A_DAY;

  // if the month is greater than 2, then we've gone past February. Include an
  // extra day if it's a leap year
  if (months >= 2 && (years % 4 == 0 && (years % 100 || years % 400 == 0)))
    ret += SECONDS_A_DAY;

  // add in leap days since 1970, but don't include the current year,
  // since we've just done that
  ret += (leapDays(years-1) - EPOCH_LEAPS) * SECONDS_A_DAY;

  // seconds a day for our day in the month
  ret += (days-1) * SECONDS_A_DAY;

  // add in the
  ret += hours * 3600;
  ret += minutes * 60;
  ret += seconds;

  return ret;
}

//TimeCode::TimeCode(const std::string&& str)
//{
//  TimeCode(str);
//}

std::ostream& operator<<(std::ostream& os, const TimeCode& tc)
{
  os << std::setw(2) << std::setfill('0') << tc.days << "/" << std::setw(2) << std::setfill('0') << (tc.months+1) << "/" << tc.years << "T"
     << std::setw(2) << std::setfill('0') << tc.hours << ":" << std::setw(2) << std::setfill('0') << tc.minutes << ":" << std::setw(2) << std::setfill('0') << tc.seconds << "Z";
  return os;
}

std::istream& operator>>(std::istream& is, TimeCode& tc)
{
  is >> tc.days;
  if (is.get() != '/')
    throw std::string("Invalid TimeCode format");
  is >> tc.months;
  if (is.get() != '/')
    throw std::string("Invalid TimeCode format");
  is >> tc.years;
  if (is.get() != 'T')
    throw std::string("Invalid TimeCode format");
  is >> tc.hours;
  if (is.get() != ':')
    throw std::string("Invalid TimeCode format");
  is >> tc.minutes;
  if (is.get() != ':')
    throw std::string("Invalid TimeCode format");
  is >> tc.seconds;
  if (is.get() != 'Z')
    throw std::string("Invalid TimeCode format");
  return is;
}
