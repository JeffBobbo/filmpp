EXEC:=omdb++

MAIN = main.o TimeCode.o Ratings.o Movie.o MovieDatabase.o

# general compiler settings
CPPFLAGS=
CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic -Werror -g
LDFLAGS=

all: $(MAIN)
	$(CXX) $(CXXFLAGS) $(MAIN) -o $(EXEC).out $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	$(warning Cleaning...)
	@$(RM) $(MAIN)

.PHONY: all clean

