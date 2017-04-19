#include "MovieDatabase.h"

const Movie& MovieDatabase::find(std::function<bool(const Movie& a, const Movie& b)> fn) const
{
  auto best = std::begin(movies);
  for (auto it = std::begin(movies); it != std::end(movies); ++it)
  {
    if (fn(*it, *best))
      best = it;
  }
  return *best;
}

const Movie* MovieDatabase::find(std::string str) const
{
  for (auto it = std::begin(movies); it != std::end(movies); ++it)
  {
    if (it->getName() == str)
      return &(*it);
  }
  return nullptr;
}

std::ostream& operator<<(std::ostream& os, const MovieDatabase& mdb)
{
  for (auto it : mdb.movies)
    os << it << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, MovieDatabase& mdb)
{
  while (is.good() && is.peek() == '"')
  {
    Movie m;
    is >> m;
    mdb.movies.push_back(m);
    is.get();
  }
  return is;
}
