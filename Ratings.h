#ifndef RATING_H_INCLUDE
#define RATING_H_INCLUDE

#include <functional>
#include <list>
#include <string>
#include <vector>

#include <stdint.h>

#include "TimeCode.h"

class Rating
{
public:
  typedef double Score;

  Rating() : when(), author(""), film(""), score(-1) {}

  const TimeCode& getDate() const { return when; }
  const std::string& getAuthor() const { return author; }
  const std::string& getFilm() const { return film; }
  const Score& getScore() const { return score; }

  friend std::ostream& operator<<(std::ostream& os, const Rating& r);
  friend std::istream& operator>>(std::istream& is, Rating& r);
private:
  TimeCode when;
  std::string author;
  std::string film;
  Score score;
};

#include <iostream>
class Ratings
{
public:
  Ratings() {}

  inline std::size_t size() const { return ratings.size(); }
  inline bool empty() const { return ratings.empty(); }
  inline const Rating& at(std::size_t i) const { return ratings.at(i); }
  inline const Rating& operator[](std::size_t i) const { return ratings[i]; }

  inline std::vector<Rating>::iterator begin() { return ratings.begin(); }
  inline std::vector<Rating>::const_iterator cbegin() const { return ratings.cbegin(); }
  inline std::vector<Rating>::iterator end() { return ratings.end(); }
  inline std::vector<Rating>::const_iterator cend() const { return ratings.cend(); }

  std::list<Rating> createFilter() const;
  static void filter(std::list<Rating>& list, std::function<bool(const Rating& m)> fn);
  const Rating& find(std::function<bool(const Rating& a, const Rating& b)> fn) const;

  inline void add(Rating& r) { ratings.push_back(r); }

  friend std::ostream& operator<<(std::ostream& os, const Ratings& ratings);
  friend std::istream& operator>>(std::istream& is, Ratings& ratings);
private:
  std::vector<Rating> ratings;
};
#endif
