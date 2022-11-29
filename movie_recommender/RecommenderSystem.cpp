#include <cmath>
#include <numeric>
#include <functional>
#include <complex>
#include <map>
#include <algorithm>
#include "RecommenderSystem.h"

sp_movie RecommenderSystem::add_movie (const std::string &name, int year,
                                       const std::vector<double> &features)
{
  sp_movie movie_ptr = std::make_shared<Movie>(name, year);
  _features.insert(std::make_pair(movie_ptr, features));
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  for (const auto & pair : _features)
    {
      if (pair.first->get_name() == name && pair.first->get_year() == year)
        {
          return pair.first;
        }
    }
  return nullptr;
}

void multiply_by_scalar(std::vector<double> &vector, double scalar)
{
  for (auto & el : vector)
    {
      el *= scalar;
    }
}

void add_vectors_into_first(std::vector<double> &lhs, const
std::vector<double>& rhs)
{
  for (int i = 0 ; i < lhs.size() ; i++)
    {
      lhs[i] += rhs[i];
    }
}

double calculate_resemblance(const std::vector<double> &lhs,
                             const std::vector<double> &rhs)
{
  return (double)
  std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), 0.0L) /
  (sqrt(inner_product(lhs.begin(), lhs.end(), lhs.begin(), 0.0L)) *
                       sqrt(inner_product(rhs.begin(), rhs.end(), rhs.begin(),
                                          0.0L)));
}

sp_movie RecommenderSystem::_stage_three_by_content (
    rank_map &ranks, const std::vector<double> &preference_vector)
{
  sp_movie recommendation = nullptr;
  double best_resemblance = -2;
  for (const auto & pair : _features)
    {
      if (ranks[pair.first] == 0)
        {
          double resemblance = calculate_resemblance(pair.second,
                                                     preference_vector);
          if (resemblance > best_resemblance)
            {
              best_resemblance = resemblance;
              recommendation = pair.first;
            }
        }
    }
  return recommendation;
}

std::vector<double>
RecommenderSystem::_stage_two_by_content (rank_map &ranks)
{
  std::vector<double> preference_vector(_features.begin()->second.size());
  for (const auto & pair : _features)
    {
      if (ranks[pair.first] != 0)
        {
          std::vector<double> temp = pair.second;
          multiply_by_scalar(temp, ranks[pair.first]);
          add_vectors_into_first(preference_vector, temp);
        }
    }
  return preference_vector;
}

void stage_one_by_content (const RSUser &user, rank_map &ranks)
{
  double sum;
  int num;
  for (const auto & el : user.get_ranks())
    {
      if (el.second != 0)
        {
          sum += el.second;
          num++;
        }
    }
  double average = sum / num;
  for (auto & el : ranks)
    {
      if (el.second != 0)
        {
          el.second -= average;
        }
    }
}


sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  rank_map ranks(user.get_ranks());
  stage_one_by_content (user, ranks);
  std::vector<double> preference_vector = _stage_two_by_content (ranks);
  return _stage_three_by_content (ranks, preference_vector);
}


bool sort_by_value_decreasing(const std::pair<sp_movie, double> &a, const
std::pair<sp_movie, double> &b)
{
  return a.second > b.second;
}


double RecommenderSystem::predict_movie_score (const RSUser &user,
                                               const sp_movie &movie, int k)
{
  std::vector<std::pair<sp_movie, double>> resemblance_map;
  for (const auto & pair : user.get_ranks())
    {
      if (pair.second != 0)
        {
          double resemblance_to_movie = calculate_resemblance(
              _features[pair.first], _features[movie]);
          resemblance_map.emplace_back(pair.first, resemblance_to_movie);
        }
    }
  std::sort(resemblance_map.begin(), resemblance_map.end(),
            sort_by_value_decreasing);
  double upper_sum = 0;
  double lower_sum = 0;
  for (int i = 0 ; i < k ; i++)
    {
      upper_sum += resemblance_map[i].second *
          user.get_ranks()[resemblance_map[i].first];
      lower_sum += resemblance_map[i].second;
    }
  return upper_sum / lower_sum;
}


sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  sp_movie recommendation = nullptr;
  double max_movie_score = -1;
  for (const auto & pair : _features)
    {
      double movie_score = predict_movie_score(user, pair.first, k);
      if (movie_score > max_movie_score)
        {
          max_movie_score = movie_score;
          recommendation = pair.first;
        }
    }
  return recommendation;
}

std::ostream& operator<< (std::ostream& os, const RecommenderSystem &other)
{
  std::vector<Movie> sorted_elements;
  for (const auto & el : other._features)
    {
      sorted_elements.push_back(*(el.first));
    }
  std::sort(sorted_elements.begin(), sorted_elements.end());
  for (const auto & movie : sorted_elements)
    {
      os << movie;
    }
  os << std::endl;
  return os;
}
