#include "Ratings.h"


std::ostream& operator<<(std::ostream& os, const Rating& r)
{
  os << r.when << ",\"" << r.author << "\",\"" << r.film << "\"," << r.score;
  return os;
}

std::istream& operator>>(std::istream& is, Rating& r)
{
  is >> r.when;
  if (is.get() != ',')
    throw std::string("Invalid Rating format");
  if (is.get() != '"')
    throw std::string("Invalid Rating format");
  r.author = "";
  while (is.peek() != '"')
    r.author += is.get();
  is.get(); // close quote
  if (is.get() != ',')
    throw std::string("Invalid Rating format");
  if (is.get() != '"')
    throw std::string("Invalid Rating format");
  r.film = "";
  while (is.peek() != '"')
    r.film += is.get();
  is.get(); // closing quote
  if (is.get() != ',')
    throw std::string("Invalid Rating format");
  is >> r.score; // last token is the score
  return is;
}

std::list<Rating> Ratings::createFilter() const
{
  std::list<Rating> l;
  for (auto m : ratings)
    l.push_back(m);
  return l;
}
void Ratings::filter(std::list<Rating>& list, std::function<bool(const Rating& m)> fn)
{
  for (auto it = std::begin(list); it != std::end(list);)
  {
    auto next = std::next(it, 1);
    if (fn(*it))
      list.erase(it);
    it = next;
  }
}

const Rating& Ratings::find(std::function<bool(const Rating& a, const Rating& b)> fn) const
{
  auto best = std::begin(ratings);
  for (auto it = std::begin(ratings); it != std::end(ratings); ++it)
  {
    if (fn(*it, *best))
      best = it;
  }
  return *best;
}

std::ostream& operator<<(std::ostream& os, const Ratings& ratings)
{
  for (auto r : ratings.ratings)
    os << r << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, Ratings& ratings)
{
  while (is.good() && is.peek() != EOF)
  {
    Rating r;
    is >> r;
    ratings.add(r);
    is.get(); // consume new line
  }
  return is;
}
