#include <string>
#include <fstream>
#include <vector>

#include "Ratings.h"
#include "Movie.h"
#include "MovieDatabase.h"

class CSV
{
public:
  CSV(const std::string& file, const char delimiter = ',', const bool header = false)
  {
    buff.open(file);
    parse(delimiter, header);
  }

  inline std::vector<std::vector<std::string> >::const_iterator begin() const
  {
    return std::begin(data);
  }
  inline std::vector<std::vector<std::string> >::const_iterator end() const
  {
    return std::end(data);
  }

  inline const std::vector<std::string>& operator[](const std::size_t n) const
  {
    return data[n];
  }

  inline std::size_t entries() const { return data.size(); }
  inline std::size_t columns() const { return data.size() ? data[0].size() : -1; }

private:
  void parse(const char& delimiter, const bool header)
  {
    if (header)
    {
      std::string head;
      std::getline(buff, head);
    }

    std::string line;
    while (std::getline(buff, line))
    {
      std::string::size_type p = 0;
      std::vector<std::string> entry;
      while (p < line.length())
      {
        if (line[p] == delimiter)
        {
          ++p;
          continue;
        }
        std::string::size_type len = 0;
        if (line[p] == '"')
        {
          ++p;
          std::string::size_type end = p;
          while (end < line.length() && line[end] != '"')
            ++end;
          len = end - p;
        }
        else
        {
          std::string::size_type end = p;
          while (end < line.length() && line[end] != delimiter)
            ++end;
          len = end - p;
        }
        entry.push_back(line.substr(p, len));
        p += len + 1;
      }
      data.push_back(entry);
    }
  }

// members
private:
  std::ifstream buff;
  std::vector<std::vector<std::string> > data;
};

MovieDatabase mdb;


#include <iostream>
int main()
{
  /*
  CSV csv("movies.txt");
  for (auto r : csv)
  {
    for (auto f : r)
      std::cout << f << " ";
    std::cout << std::endl;
  }
   */

  TimeCode tcEpoch;
  TimeCode tcNowIsh;
  std::istringstream is("02/04/2400T15:14:30Z");
  is >> tcNowIsh;

  if (tcEpoch != 0)
    std::cout << "tcEpoch isn't 0 but is " << std::time_t(tcEpoch)<< ", diff: " << (0 - std::time_t(tcEpoch)) << std::endl;
  if (tcNowIsh != 13577469270)
    std::cout << "tcNowIsh isn't 13577469270 but is " << std::time_t(tcNowIsh)<< ", diff: " << (13577469270 - std::time_t(tcNowIsh)) << std::endl;

  std::ifstream ifs("ratings.txt");
  std::string line;
  while (std::getline(ifs, line))
  {
    std::istringstream is(line);
    Rating r;
    is >> r;
    mdb.ratings.add(r);
  }
  std::cout << mdb.ratings.size() << std::endl;
  std::cout << mdb.ratings.at(0) << std::endl;

  std::cout << "The smallest score in the ratings is: " << static_cast<uint>(mdb.ratings.min()) << std::endl;
  std::cout << "The biggest score in the ratings is: " << static_cast<uint>(mdb.ratings.max()) << std::endl;

  std::cout << std::endl << std::endl;
  try
  {
    std::ifstream ifs("movies.txt");
    ifs >> mdb;
  }
  catch (std::string e)
  {
    std::cout << e << std::endl;
  }
  mdb.sort();
  std::cout << mdb << std::endl;

  {
    std::list<Movie> filmnoir = mdb.createFilter();
    MovieDatabase::filter(filmnoir, [](const Movie& m) { return m.getGenre()[Genre::CATEGORY::FILM_NOIR]; });
    filmnoir.sort([](const Movie& a, const Movie& b) { return a.getRuntime() > b.getRuntime(); });
    std::list<Movie>::const_iterator it = std::next(std::begin(filmnoir), 2);
    if (it != std::end(filmnoir))
      std::cout << *it << std::endl;
  }

  {
    std::list<Movie> scifi = mdb.createFilter();
    MovieDatabase::filter(scifi, [](const Movie& m) { return m.getGenre()[Genre::CATEGORY::SCI_FI]; });
    scifi.sort([](const Movie& a, const Movie& b) { return a.ratingHighest() > b.ratingHighest(); });
    std::list<Movie>::const_iterator it = std::next(std::begin(scifi), 9);
    if (it != std::end(scifi))
      std::cout << *it << std::endl;
  }

  {
    std::string film = mdb.ratings.find([](const Rating& a, const Rating& b) { return a.getScore() > b.getScore(); }).getFilm();
    std::cout << *(mdb.find(film)) << std::endl;
  }

  {
    std::cout << mdb.find([](const Movie& a, const Movie& b) { return a.getName().length() > b.getName().length(); }) << std::endl;
  }

  {
    std::list<Rating> list = mdb.ratings.createFilter();
    list.sort([](const Rating& a, const Rating& b) { return a.getDate() < b.getDate(); });
    std::list<Rating>::const_iterator it = std::next(std::begin(list), 100);
    if (it != std::end(list))
      std::cout << *it << std::endl;
  }

  /*
  Movie m;
  std::istringstream ism("\"Strangers on a Train\",1951,\"PG\",\"Crime/Film-Noir/Romance\",101,0,0");
  ism >> m;
  std::cout << m << std::endl;
  std::cout << m.getGenre() << std::endl;
  std::cout << m.getGenre()[Genre::CATEGORY::FILM_NOIR] << std::endl;
  */

  /*

  try
  {
    Genre g;
    std::string f("Action/Adventure/Fantasy");
    std::istringstream is2(f);
    is2 >> g;
    std::cout << g << std::endl;
  }
  catch (std::string s)
  {
    std::cout << s << std::endl;
  }
   */
}
