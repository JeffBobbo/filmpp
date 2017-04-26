#ifndef MOVIE_H_INCLUDE
#define MOVIE_H_INCLUDE

#include <string>
#include <sstream>

#include <stdint.h>

#include "Ratings.h"

enum class AgeRating
{
  APPROVED = 0,
  G,
  M,
  N_A,
  NOT_RATED,
  PASSED,
  PG,
  PG_13,
  R,
  TV_14,
  UNRATED,
  X
};

std::ostream& operator<<(std::ostream& os, const AgeRating& r);
// TODO: Replace this with an operator>> implementation
AgeRating ageFrom_string(const std::string& str);


class Genre
{
public:
  enum class CATEGORY
  {
    ACTION = 0x1,
    ADVENTURE = 0x2,
    ANIMATION = 0x4,
    BIOGRAPHY = 0x8,
    COMEDY = 0x10,
    CRIME = 0x20,
    DRAMA = 0x40,
    FAMILY = 0x80,
    FANTASY = 0x100,
    FILM_NOIR = 0x200,
    HISTORY = 0x400,
    HORROR = 0x800,
    MUSIC = 0x1000,
    MUSICAL = 0x2000,
    MYSTERY = 0x4000,
    ROMANCE = 0x8000,
    SCI_FI = 0x10000,
    THRILLER = 0x20000,
    WAR = 0x40000,
    WESTERN = 0x80000
  };

  Genre() : flags(0) {}
  Genre(uint32_t g) : flags(g) {}

  bool operator[](const Genre::CATEGORY c) const { return (flags & static_cast<uint32_t>(c)) != 0; }

  friend std::ostream& operator<<(std::ostream& os, const Genre& g);
  friend std::istream& operator>>(std::istream& is, Genre& g);
private:
  uint32_t flags;
};

std::ostream& operator<<(std::ostream& os, const Genre::CATEGORY& c);
Genre::CATEGORY categoryFrom_string(const std::string& str);

class Movie
{
public:
  Movie() : count(0), totalScore(-1), average(0) {}

  const std::string& getName() const { return name; }
  const Genre& getGenre() const { return genre; }
  int16_t getRuntime() const { return runtime; }

  Rating::Score ratingHighest() const;
  std::size_t   ratingCount() const;
  Rating::Score ratingTotal() const;
  Rating::Score ratingAverage() const;

  inline bool operator<(const Movie& other) const { return year < other.year || (year == other.year && name < other.name); }
  inline bool operator>(const Movie& other) const { return year > other.year || (year == other.year && name > other.name); }
  inline bool operator<=(const Movie& other) const { return !(*this > other); }
  inline bool operator>=(const Movie& other) const { return !(*this < other); }
  inline bool operator==(const Movie& other) const { return year == other.year && name == other.name; }
  inline bool operator!=(const Movie& other) const { return !(*this == other); }

  friend std::ostream& operator<<(std::ostream& os, const Movie& m);
  friend std::istream& operator>>(std::istream& is, Movie& m);
private:
  void calcRatings() const;
  std::string name;
  int16_t year;
  AgeRating age;
  Genre genre;
  int16_t runtime;

  mutable std::size_t count;
  mutable Rating::Score totalScore;
  mutable Rating::Score average;
};

#endif
