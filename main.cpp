/*main.cpp*/

//
// Netflix movie analysis: top-10 by rating and # of reviews.
//
// Prof. Joe Hummel
// Windows with Visual Studio
// U. of Illinois, Chicago
// CS341, Spring 2017
// Project #01: Solution
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;


//
// Movie class:
//
class Movie
{
private:
  int     ID;
  string  Name;
  int     PubYear;
  int     NumReviews;
  double  SumRatings;
  double  AvgRating;

public:
  //
  // constructor:
  //
  Movie(int id, string name, int pubyear)
    : ID(id), Name(name), PubYear(pubyear)
  {
    NumReviews = 0;
    SumRatings = 0.0;
    AvgRating = 0.0;
  }

  //
  // getters/setters:
  //
  int getMovieID() const
  {
    return ID;
  }

  string getMovieName() const
  {
    return Name;
  }

  int getPubYear() const
  {
    return PubYear;
  }

  int getNumReviews() const
  {
    return NumReviews;
  }

  void increaseNumReviews()
  {
    NumReviews++;
  }

  void increaseSumRatings(int rating)
  {
    SumRatings += rating;
  }

  //
  // returns average rating for this movie:
  //
  double getAvgRating() const
  {
    if (NumReviews == 0)
      return 0.0;
    else
      return SumRatings / NumReviews;
  }
};


//
// FindMovie: searches the vector for the movie with matching
// movie id; returns index >= 0 if found, -1 if not.
//
int FindMovie(const vector<Movie>& movies, int movieID)
{
  for (size_t i = 0; i < movies.size(); ++i)
  {
    if (movies[i].getMovieID() == movieID)
      return i;
  }

  // if get here, not found:
  return -1;
}



//
// InputMovies: inputs the movies from the given file, and stores
// the data in a vector of Movie objects.  The vector is returned.
//
vector<Movie> InputMovies(string filename)
{
  ifstream file(filename);
  vector<Movie> movies;
  string line;

  if (!file.good())
  {
    cout << "**Error: cannot open file: " << filename << endl;
    std::terminate();
  }

  getline(file, line);  // discard first line (column headers):

  while (getline(file, line))
  {
    stringstream  ss(line);

    // format: MovieID,MovieName,PubYear
    string movieID, name, year;

    getline(ss, movieID, ',');
    getline(ss, name, ',');
    getline(ss, year);

    Movie m(stoi(movieID), name, stoi(year));

    movies.push_back(m);
  }

  //
  // done, return input:
  //
  return movies;
}


//
// ProcessReviews: inputs reviews from the given file, and for each
// review updates the corresponding movie: # of reviews and sum of
// ratings.  Returns the # of reveiws processed.
//
int ProcessReviews(string filename, vector<Movie>& movies)
{
  ifstream file(filename);
  int N = 0;
  string line;

  if (!file.good())
  {
    cout << "**Error: cannot open file: " << filename << endl;
    std::terminate();
  }

  getline(file, line);  // discard first line (column headers):

  while (getline(file, line))
  {
    N++;

    stringstream  ss(line);

    // format: MovieID,UserID,Rating,ReviewDate
    string movieID, userID, rating;

    getline(ss, movieID, ',');
    getline(ss, userID, ',');
    getline(ss, rating);
    // ignore review date, we don't need it:

    //
    // now that we've parsed the review, search for corresponding
    // movie and update:
    //
    int i = FindMovie(movies, stoi(movieID));
    if (i >= 0)
    {
      movies[i].increaseNumReviews();
      movies[i].increaseSumRatings(stoi(rating));
    }
  }

  //
  // done, return # of reviews processed:
  //
  return N;
}


//
// SortMoviesByAvgRatingDesc: sorts the movies by their average rating, in
// descending order; secondary sort by Movie name when the avgs are equal.
//
// NOTE: since N is small, simple selection sort is used.
//
void SortMoviesByAvgRatingDesc(vector<Movie>& movies)
{
  sort(movies.begin(),
    movies.end(),
    [](const Movie& m1, const Movie& m2)
      {
      if (m1.getAvgRating() > m2.getAvgRating())
        return true;
      else if (m1.getAvgRating() < m2.getAvgRating())
        return false;
      else // reviews are equal, secondary sort by movie name:
      {
        if (m1.getMovieName() < m2.getMovieName())
          return true;
        else
          return false;
      }
    }
  );
}


//
// SortMoviesByNumReviewsDesc: sorts the movies by the # of reviews, in
// descending order; secondary sort by Movie name when the avgs are equal.
//
// NOTE: since N is small, simple selection sort is used.
//
void SortMoviesByNumReviewsDesc(vector<Movie>& movies)
{
  sort(movies.begin(),
    movies.end(),
    [](const Movie& m1, const Movie& m2)
    {
      if (m1.getNumReviews() > m2.getNumReviews())
        return true;
      else if (m1.getNumReviews() < m2.getNumReviews())
        return false;
      else // reviews are equal, secondary sort by movie name:
      {
        if (m1.getMovieName() < m2.getMovieName())
          return true;
        else
          return false;
      }
    }
  );
}


//
// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
//
string getFileName()
{
  string filename;

  // input filename from the keyboard:
  getline(cin, filename);

  // make sure filename exists and can be opened:
  ifstream file(filename);
  if (!file.good())
  {
    cout << "**Error: cannot open file: " << filename << endl;
    std::terminate();
  }

  return filename;
}


int main()
{
  // get filenames from the user/stdin:
  string MoviesFileName = getFileName();
  string ReviewsFileName = getFileName();

  // input movie data:
  vector<Movie> movies = InputMovies(MoviesFileName);

#ifdef NEVER
  for (int i = 0; i < movies.size(); ++i)
  {
    cout << movies[i].ID << ": '"
      << movies[i].Name << "', "
      << movies[i].PubYear << endl;
  }
#endif

  // input & process review data:
  int numReviews = ProcessReviews(ReviewsFileName, movies);

  // output results:
  cout << std::fixed;
  cout << std::setprecision(6);

  cout << "Movies: " << movies.size() << endl;
  cout << "Reviews " << numReviews << endl;

  SortMoviesByAvgRatingDesc(movies);

  // top 10 by avg rating:
  cout << "**Top-10 by Rating**" << endl;

  for (int i = 0; i < 10; ++i)
  {
    cout << i + 1 << ": "
      << movies[i].getAvgRating() << ",'"
      << movies[i].getMovieName() << "'" << endl;
  }

  SortMoviesByNumReviewsDesc(movies);

  // top 10 by # of reviews:
  cout << "**Top-10 by Num Reviews**" << endl;

  for (int i = 0; i < 10; ++i)
  {
    cout << i + 1 << ": "
      << movies[i].getNumReviews() << ",'"
      << movies[i].getMovieName() << "'" << endl;
  }

  //
  // done:
  //
  return 0;
}
