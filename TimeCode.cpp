#include "TimeCode.h"

#include <chrono>
#include <iomanip> // std::setw, std::setfill
#include <utility> // std::move

std::time_t leapDays(const std::time_t& year)
{
  return
    (year)/4 // leap year every 4 years
  - (year)/100 // but not every 100, so take away those
  + (year)/400; // but it is every 400, so add those back in
}

const std::time_t SECONDS_A_DAY = 86400;
const std::time_t SECONDS_A_YEAR = SECONDS_A_DAY * 365;
const std::time_t EPOCH = 1970;
// number of leap days up to 1970-01-01
const std::time_t EPOCH_LEAPS = leapDays(EPOCH-1);
const std::time_t MONTHS[] = {
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
  31  // dec
};

bool isLeapYear(const std::time_t& year)
{
  return year % 4 == 0 && (year % 100 != 0 || year % 400);
}

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

TimeCode::TimeCode(std::time_t t) : TimeCode()
{
  seconds = t % 60;
  t -= seconds;

  minutes = (t / 60) % 60;
  t -= minutes * 60;
  hours = (t / 3600) % 24;
  t -= hours * 3600;

  days = t / 86400;
  years = days / 365;
  days -= years * 365;
  std::time_t leaps = leapDays(years-1);
  years += 1970;

  days -= leaps;
  if (days < 0)
  {
    --years;
    days += 365;
  }

  std::time_t mdays = 0;
  std::time_t i = 0;
  for (; i < 12; ++i)
  {
    if (mdays + MONTHS[i] < days)
      mdays += MONTHS[i];
    else
      break;
  }
  months = i;
  days -= mdays;
  ++days;
}

TimeCode::operator std::time_t() const
{
  std::time_t y = years - EPOCH;

  // seconds for each year since EPOCH
  std::time_t ret = y * SECONDS_A_YEAR;

  // seconds for each day of each month of this year
  for (std::time_t i = 0; i < months-1; ++i)
    ret += MONTHS[i] * SECONDS_A_DAY;

  // if the month is greater than 2, then we've gone past February. Include an
  // extra day if it's a leap year
  if (months >= 2 && isLeapYear(years))
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

TimeCode& TimeCode::operator+=(const TimeCode& other)
{
  seconds += other.seconds;
  while (seconds < 0)
  {
    --minutes;
    seconds += 60;
  }
  while (seconds > 59)
  {
    ++minutes;
    seconds -= 60;
  }

  minutes += other.minutes;
  while (minutes < 0)
  {
    --hours;
    minutes += 60;
  }
  while (minutes > 59)
  {
    ++hours;
    minutes -= 60;
  }

  hours += other.hours;
  while (hours < 0)
  {
    --days;
    hours += 24;
  }
  while (hours > 23)
  {
    ++days;
    hours -= 24;
  }

  // -1 days because day range is 1..31
  --days;
  for (std::time_t m = 0; m < months; ++m)
    days += MONTHS[m];
  for (std::time_t m = 0; m < other.months; ++m)
    days += MONTHS[m];
  days += other.days;
  months = 0;

  while (days < 1)
  {
    days += 365;
    if ((0 - days) > 60 && isLeapYear(years))
      days += 1;
    --years;
  }
  while (days > (isLeapYear(years) ? 366 : 365))
  {
    days -= 365;
    if (days > 60 && isLeapYear(years))
      --days;
    ++years;
  }

  std::time_t m = 0;
  while (days > MONTHS[m])
    days -= MONTHS[m++];
  months = m;

  return *this;
}

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
