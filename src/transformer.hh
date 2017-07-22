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
    void replace_color(cv::Vec3b& color);
    static uchar bdigit(uchar value, size_t bit_index);

  private:
    cv::Mat& img_;
    Octree colors_;
  };
}
