#include <transformer.hh>

allrgb::Transformer::Transformer(cv::Mat& input)
  : img_(input)
  , colors_(9)
{}

cv::Mat&
allrgb::Transformer::img_get() const
{
  return img_;
}

void
allrgb::Transformer::operator()()
{
  for (auto it = img_.begin<cv::Vec3b>(); it != img_.end<cv::Vec3b>(); ++it)
    replace_color_(*it);
}

void
allrgb::Transformer::replace_color_(cv::Vec3b& color)
{
  const uchar blue = color[0];
  const uchar green = color[1];
  const uchar red = color[2];

  uchar new_red = 0;
  uchar new_green = 0;
  uchar new_blue = 0;

  size_t octree_index = 0;

  for (unsigned i = 0; i < 8; ++i)
  {
    size_t child = ocnode_index_(red, green, blue, i);
    octree_index = colors_.index_child(octree_index, child);

    if (colors_.at(octree_index) > 0)
      choose_child_(new_red, new_green, new_blue, child);
    else
    {
      // TODO lookup for another color
    }

    // TODO decrement sub_leaves amount
  }

  color[2] = new_blue;
  color[1] = new_green;
  color[0] = new_red;
}

// bit_index = 0 is the LSB
uchar
allrgb::Transformer::bdigit_(const uchar value, const size_t bit_index)
{
  assert(bit_index < 8);

  const uchar mask = 0b00000001 << bit_index;
  return (value & mask) >> bit_index;
}

size_t
allrgb::Transformer::ocnode_index_(const uchar r, const uchar g, const uchar b,
                                   const size_t ocnode_depth)
{
  assert(ocnode_depth < 8);

  const size_t bindex = 7 - ocnode_depth;

  size_t res = 0;
  res = bdigit_(r, bindex);
  res = (res << 1) | bdigit_(g, bindex);
  res = (res << 1) | bdigit_(b, bindex);
  return res;
}

void
allrgb::Transformer::choose_child_(uchar& r, uchar& g, uchar& b,
                                   const size_t nb_child)
{
  assert(nb_child < 8);

  const uchar red_digit = (nb_child & 0b00000100) >> 2;
  const uchar green_digit = (nb_child & 0b00000010) >> 1;
  const uchar blue_digit = nb_child & 0b00000001;

  r = (r << 1) | red_digit;
  g = (g << 1) | green_digit;
  b = (b << 1) | blue_digit;
}
