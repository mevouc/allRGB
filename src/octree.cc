# include <octree.hh>

# include <cassert>
# include <cmath>

allrgb::Octree::Octree(unsigned depth)
  : data_((std::pow(8ul, depth) - 1) / (8ul - 1))
  , nb_leaves_(std::pow(8ul, depth - 1))
{
  init_();
}

void
allrgb::Octree::init_()
{
  init_(0, nb_leaves_);
}

void
allrgb::Octree::init_(unsigned long index, unsigned long sub_leaves)
{
  data_.at(index) = sub_leaves;

  if (sub_leaves <= 1)
    return;

  for (unsigned i = 0; i < 8; ++i)
    init_(8 * index + 1 + i, sub_leaves / 8);
}
