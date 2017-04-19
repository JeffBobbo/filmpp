#include "Movie.h"

#include "MovieDatabase.h"

std::ostream& operator<<(std::ostream& os, const AgeRating& r)
{
  switch (r)
  {
    case AgeRating::APPROVED:
      os << "APPROVED";
    break;
    case AgeRating::G:
      os << "G";
    break;
    case AgeRating::M:
      os << "M";
    break;
    case AgeRating::N_A:
      os << "N/A";
    break;
    case AgeRating::NOT_RATED:
      os << "NOT RATED";
    break;
    case AgeRating::PASSED:
      os << "PASSED";
    break;
    case AgeRating::PG:
      os << "PG";
    break;
    case AgeRating::PG_13:
      os << "PG-13";
    break;
    case AgeRating::R:
      os << "R";
    break;
    case AgeRating::TV_14:
      os << "TV-14";
    break;
    case AgeRating::UNRATED:
      os << "UNRATED";
    break;
    case AgeRating::X:
      os << "X";
    break;
  }
  return os;
}
AgeRating ageFrom_string(const std::string& str)
{
  if (str == "APPROVED")
    return AgeRating::APPROVED;
  else if (str == "G")
    return AgeRating::G;
  else if (str == "M")
    return AgeRating::M;
  else if (str == "N/A")
    return AgeRating::N_A;
  else if (str == "NOT RATED")
    return AgeRating::NOT_RATED;
  else if (str == "PASSED")
    return AgeRating::PASSED;
  else if (str == "PG")
    return AgeRating::PG;
  else if (str == "PG-13")
    return AgeRating::PG_13;
  else if (str == "R")
    return AgeRating::R;
  else if (str == "TV-14")
    return AgeRating::TV_14;
  else if (str == "UNRATED")
    return AgeRating::UNRATED;
  else if (str == "X")
    return AgeRating::X;
  else
    throw std::string("Invalid age rating");
  return AgeRating::NOT_RATED;
}

std::ostream& operator<<(std::ostream& os, const Genre::CATEGORY& c)
{
  switch (c)
  {
    case Genre::CATEGORY::ACTION:
      return os << "Action";
    case Genre::CATEGORY::ADVENTURE:
      return os << "Adventure";
    case Genre::CATEGORY::ANIMATION:
      return os << "Animation";
    case Genre::CATEGORY::BIOGRAPHY:
      return os << "Biography";
    case Genre::CATEGORY::COMEDY:
      return os << "Comedy";
    case Genre::CATEGORY::CRIME:
      return os << "Crime";
    case Genre::CATEGORY::DRAMA:
      return os << "Drama";
    case Genre::CATEGORY::FAMILY:
      return os << "Family";
    case Genre::CATEGORY::FANTASY:
      return os << "Fantasy";
    case Genre::CATEGORY::FILM_NOIR:
      return os << "Film-Noir";
    case Genre::CATEGORY::HISTORY:
      return os << "History";
    case Genre::CATEGORY::HORROR:
      return os << "Horror";
    case Genre::CATEGORY::MUSIC:
      return os << "Music";
    case Genre::CATEGORY::MUSICAL:
      return os << "Musical";
    case Genre::CATEGORY::MYSTERY:
      return os << "Mystery";
    case Genre::CATEGORY::ROMANCE:
      return os << "Romance";
    case Genre::CATEGORY::SCI_FI:
      return os << "Sci-Fi";
    case Genre::CATEGORY::THRILLER:
      return os << "Thriller";
    case Genre::CATEGORY::WAR:
      return os << "War";
    case Genre::CATEGORY::WESTERN:
      return os << "Western";
  }
  return os;
}
Genre::CATEGORY categoryFrom_string(const std::string& str)
{
  if (str == "Action")
    return Genre::CATEGORY::ACTION;
  else if (str == "Adventure")
    return Genre::CATEGORY::ADVENTURE;
  else if (str == "Animation")
    return Genre::CATEGORY::ANIMATION;
  else if (str == "Biography")
    return Genre::CATEGORY::BIOGRAPHY;
  else if (str == "Comedy")
    return Genre::CATEGORY::COMEDY;
  else if (str == "Crime")
    return Genre::CATEGORY::CRIME;
  else if (str == "Drama")
    return Genre::CATEGORY::DRAMA;
  else if (str == "Family")
    return Genre::CATEGORY::FAMILY;
  else if (str == "Fantasy")
    return Genre::CATEGORY::FANTASY;
  else if (str == "Film-Noir")
    return Genre::CATEGORY::FILM_NOIR;
  else if (str == "History")
    return Genre::CATEGORY::HISTORY;
  else if (str == "Horror")
    return Genre::CATEGORY::HORROR;
  else if (str == "Music")
    return Genre::CATEGORY::MUSIC;
  else if (str == "Musical")
    return Genre::CATEGORY::MUSICAL;
  else if (str == "Mystery")
    return Genre::CATEGORY::MYSTERY;
  else if (str == "Romance")
    return Genre::CATEGORY::ROMANCE;
  else if (str == "Sci-Fi")
    return Genre::CATEGORY::SCI_FI;
  else if (str == "Thriller")
    return Genre::CATEGORY::THRILLER;
  else if (str == "War")
    return Genre::CATEGORY::WAR;
  else if (str == "Western")
    return Genre::CATEGORY::WESTERN;
  throw std::string("Unknown category: ") + str;
}

std::ostream& operator<<(std::ostream& os, const Genre& g)
{
  bool b = false;
  for (uint32_t i = 1; i <= static_cast<uint32_t>(Genre::CATEGORY::WESTERN); i<<=1)
  {
    if (g.flags & i)
    {
      if (b)
        os << "/";
      os << static_cast<Genre::CATEGORY>(i);
      b = true;
    }
  }
  return os;
}

std::istream& operator>>(std::istream& is, Genre& g)
{
  while (is.good() && is.peek() != '"')
  {
    std::stringstream ss;
    while (is.good() && is.peek() != '/' && is.peek() != '"')
    {
      char c = is.get();
      if (is.good())
        ss << c;
    }
    std::string s = ss.str();
    g.flags |= static_cast<uint32_t>(categoryFrom_string(s));
    if (is.peek() == '/')
      is.get();
  }
  return is;
}

Rating::Score Movie::ratingHighest() const
{
  Rating::Score best = 0;
  for (auto it : mdb.ratings)
  {
    if (it.getFilm() == getName() && it.getScore() > best)
      best = it.getScore();
  }
  return best;
}

std::ostream& operator<<(std::ostream& os, const Movie& m)
{
  os << "\"" << m.name << "\"," << m.year << ",\"" << m.age << "\"," << m.genre << "\"," << m.runtime << ",0,0";
  return os;
}

std::istream& operator>>(std::istream& is, Movie& m)
{
  if (is.get() != '"')
    throw std::string("Invalid Movie format");
  m.name = "";
  while (is.peek() != '"')
    m.name += is.get();
  is.get();
  if (is.get() != ',')
    throw std::string("Invalid Movie format");
  is >> m.year;
  if (is.get() != ',')
    throw std::string("Invalid Movie format");
  if (is.get() != '"')
    throw std::string("Invalid Movie format");
  std::string rating;
  while (is.peek() != '"')
    rating += is.get();
  m.age = ageFrom_string(rating);
  if (is.get() != '"')
    throw std::string("Invalid Movie format");
  if (is.get() != ',')
    throw std::string("Invalid Movie format");
  if (is.get() != '"')
    throw std::string("Invalid Movie format");
  is >> m.genre;
//  std::stringstream ss;
//  while (is.peek() != '"')
//    ss << static_cast<char>(is.get());
//  m.genre = ss.str();
  if (is.get() != '"')
    throw std::string("Invalid Movie format");
  if (is.get() != ',')
    throw std::string("Invalid Movie format");
  is >> m.runtime;
  if (is.get() != ',')
    throw std::string("Invalid Movie format");
  if (is.get() != '0')
    throw std::string("Invalid Movie format");
  if (is.get() != ',')
    throw std::string("Invalid Movie format");
  if (is.get() != '0')
    throw std::string("Invalid Movie format");
  return is;
}
