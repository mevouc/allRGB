#pragma once

# include <opencv2/core.hpp>

namespace allrgb
{
  int run(const std::string& input, const std::string& output);
  bool check(const std::string& input);
  bool check(const cv::Mat& img);
}
