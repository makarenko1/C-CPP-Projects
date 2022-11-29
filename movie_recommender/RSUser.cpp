
// don't change those includes
#include "RSUser.h"

#include <utility>
#include <algorithm>
#include <map>
#include "RecommenderSystem.h"


RSUser::RSUser (const std::string &username, rank_map ranks,
                RecommenderSystem &recommender_system) :
                _username(username), _ranks(std::move(ranks)),
                _recommender_system(recommender_system)
                {}

const std::string & RSUser::get_name () const
{return _username;}

void RSUser::add_movie_to_rs (const std::string &name, int year,
                              const std::vector<double> &features, double rate)
{
  sp_movie movie = _recommender_system.add_movie(name, year, features);
  _ranks.insert(std::make_pair(movie, rate));
}

rank_map & RSUser::get_ranks ()
{return _ranks;}

rank_map RSUser::get_ranks () const
{return _ranks;}

sp_movie RSUser::get_recommendation_by_content () const
{
  return _recommender_system.recommend_by_content(*this);
}

double RSUser::get_prediction_score_for_movie (const std::string &name,
                                               int year, int k) const
{
  sp_movie movie = _recommender_system.get_movie(name, year);
  return _recommender_system.predict_movie_score(*this, movie, k);
}

sp_movie RSUser::get_recommendation_by_cf (int k) const
{
  return _recommender_system.recommend_by_cf(*this, k);
}

std::ostream& operator<< (std::ostream& os, const RSUser &other)
{
  std::cout << "name: " << other._username << "\n";
  std::vector<Movie> sorted_elements;
  for (const auto& el : other._ranks)
    {
      sorted_elements.push_back(*(el.first));
    }
  std::sort(sorted_elements.begin(), sorted_elements.end());
  for (const auto& movie : sorted_elements)
    {
      os << movie;
    }
  os << std::endl;
  return os;
}