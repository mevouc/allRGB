#include <algorithm>
#include <random>
#include <vector>

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
  assert(colors_.at(0) == 4096 * 4096);

  std::vector<cv::Point> points;
  points.reserve(4096 * 4096);
  for (size_t y = 0; y < 4096; ++y)
    for (size_t x = 0; x < 4096; ++x)
      points.emplace_back(x, y);
  std::random_device rd;
  std::shuffle(points.begin(), points.end(), std::mt19937(rd()));

  while (!points.empty())
  {
    cv::Point& point = *points.end();
    replace_color_(img_.at<cv::Vec3b>(point));
    points.pop_back();
  }

  assert(colors_.at(0) == 0);
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
    const size_t perfect_child = ocnode_index_(red, green, blue, i);
    const size_t prev_octree_index = octree_index;
    octree_index = colors_.index_child(prev_octree_index, perfect_child);

    size_t last_chosen = perfect_child;
    size_t nb_try = 0;
    while (colors_.at(octree_index) <= 0)
    {
      last_chosen = next_lookup_(perfect_child, ++nb_try);
      octree_index = colors_.index_child(prev_octree_index, last_chosen);
    }
    choose_child_(new_red, new_green, new_blue, last_chosen);
  }

  colors_.delete_leaf(octree_index);

  color[0] = new_blue;
  color[1] = new_green;
  color[2] = new_red;
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

size_t
allrgb::Transformer::next_lookup_(const size_t perfect, const size_t nb_try)
{
  assert(perfect < 8 && nb_try < 8);

  const static size_t lookup_table[8][8] = {
    {0, 1, 4, 5, 2, 3, 6, 7},
    {1, 0, 5, 4, 3, 2, 7, 6},
    {2, 3, 6, 7, 0, 1, 4, 5},
    {3, 2, 7, 6, 1, 0, 5, 4},
    {4, 5, 0, 1, 6, 7, 2, 3},
    {5, 4, 1, 0, 7, 6, 3, 2},
    {6, 7, 2, 3, 4, 5, 0, 1},
    {7, 6, 3, 2, 5, 4, 1, 0}
  };

  return lookup_table[perfect][nb_try];
}
