#include <transformer.hh>

allrgb::Transformer::Transformer(cv::Mat& input)
  : img_(input)
{}

cv::Mat&
allrgb::Transformer::img_get() const
{
  return img_;
}

void
allrgb::Transformer::operator()()
{}
