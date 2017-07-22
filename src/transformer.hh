#pragma once

# include <opencv2/core.hpp>
# include <opencv2/imgcodecs.hpp>
# include <opencv2/imgproc.hpp>

# include <octree.hh>

namespace allrgb
{
  class Transformer
  {
  public:
    Transformer(cv::Mat& input);

    cv::Mat& img_get() const;

    void operator()();

  private:
    cv::Mat& img_;
    Octree colors_;
  };
}
