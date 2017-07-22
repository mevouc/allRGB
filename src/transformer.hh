#pragma once

# include <fwd.hh>

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
  };
}
