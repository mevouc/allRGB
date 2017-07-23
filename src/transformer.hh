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
    void replace_color_(cv::Vec3b& color);

    static uchar bdigit_(const uchar value, const size_t bit_index);
    static size_t ocnode_index_(const uchar r, const uchar g, const uchar b,
                                const size_t ocnode_depth);
    static void choose_child_(uchar& r, uchar& g, uchar& b,
                              const size_t nb_child);

  private:
    cv::Mat& img_;
    Octree colors_;
  };
}
