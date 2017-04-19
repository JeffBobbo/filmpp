#ifndef MOVIEDATABASE_H_INCLUDE
#define MOVIEDATABASE_H_INCLUDE

#include <list>
#include <vector>
#include <algorithm>
#include <functional>

#include "Movie.h"

class MovieDatabase
{
public:
  MovieDatabase() {};

  inline std::size_t size() const { return movies.size() ; }
  inline bool empty() const { return movies.empty(); }

  std::list<Movie> createFilter() const
  {
    std::list<Movie> l;
    for (auto m : movies)
      l.push_back(m);
    return l;
  }
  static void filter(std::list<Movie>& list, std::function<bool(const Movie& m)> fn)
  {
    for (auto it = std::begin(list); it != std::end(list);)
    {
      auto next = std::next(it, 1);
      if (!fn(*it))
        list.erase(it);
      it = next;
    }
  }


  void sort() { std::sort(std::begin(movies), std::end(movies)); }
  void sort(std::function<bool(const Movie& a, const Movie& b)> fn)
  {
    std::sort(std::begin(movies), std::end(movies), fn);
  }
  const Movie& find(std::function<bool(const Movie& a, const Movie& b)> fn) const;
  const Movie* find(std::string str) const;


  friend std::ostream& operator<<(std::ostream& os, const MovieDatabase& mdb);
  friend std::istream& operator>>(std::istream& is, MovieDatabase& mdb);

  Ratings ratings;
private:
  std::vector<Movie> movies;
};
extern MovieDatabase mdb;

#endif
