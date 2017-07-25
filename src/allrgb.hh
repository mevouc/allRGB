#pragma once

# include <opencv2/core.hpp>
# include <vector>

namespace allrgb
{
  int run(const std::string& input, const std::string& output,
          const bool random, const bool verbose);

  cv::Mat scale(const cv::Mat& img);

  using pair_vect_it =
    std::vector<std::pair<const double, const cv::Size>>::iterator;

  pair_vect_it get_closest_size(pair_vect_it begin, pair_vect_it end,
                                 double scale);

  bool check(const std::string& input);

  bool check(const cv::Mat& img);
}
