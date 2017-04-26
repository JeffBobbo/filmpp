#include <string>
#include <fstream>
#include <vector>

#include "Ratings.h"
#include "Movie.h"
#include "MovieDatabase.h"

MovieDatabase mdb;

#include <iostream>
int main()
{
  /*
  TimeCode tcEpoch;
  TimeCode tcNowIsh;
  std::istringstream is("02/04/2400T15:14:30Z");
  is >> tcNowIsh;

  if (static_cast<std::time_t>(tcEpoch) != 0)
    std::cout << "tcEpoch isn't 0 but is " << std::time_t(tcEpoch)<< ", diff: " << (0 - std::time_t(tcEpoch)) << std::endl;
  if (static_cast<std::time_t>(tcNowIsh) != 13577469270)
    std::cout << "tcNowIsh isn't 13577469270 but is " << std::time_t(tcNowIsh)<< ", diff: " << (13577469270 - std::time_t(tcNowIsh)) << std::endl;

  TimeCode tcDay(86400);
  TimeCode tcHour(3600);

  TimeCode tcDayHour = tcDay;
  tcDayHour -= tcHour;
  std::cout << tcDay << " + " << tcHour << " = " << tcDayHour << std::endl;
  std::cout << static_cast<std::time_t>(tcDay) << " + " << static_cast<std::time_t>(tcHour) << " = " << static_cast<std::time_t>(tcDayHour) << std::endl;
  */


  try
  {
    std::ifstream ifsr("ratings.txt");
    ifsr >> mdb.ratings;
    ifsr.close();
    std::ifstream ifsm("movies.txt");
    ifsm >> mdb;
    ifsm.close();
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
    scifi.sort([](const Movie& a, const Movie& b) { return a.ratingAverage() > b.ratingAverage(); });
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
