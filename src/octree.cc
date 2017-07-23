# include <octree.hh>

# include <cassert>
# include <cmath>

allrgb::Octree::Octree(size_t depth)
  : data_((std::pow(8ul, depth) - 1) / (8ul - 1))
  , nb_leaves_(std::pow(8ul, depth - 1))
{
  init_();
}

const size_t&
allrgb::Octree::at(const size_t index) const
{
  assert(index <= data_.size());

  return data_.at(index);
}

const size_t&
allrgb::Octree::operator[](const size_t index) const
{
  return data_[index];
}

size_t
allrgb::Octree::index_child(const size_t index, const size_t nb_child)
{
  assert(nb_child < 8);

  size_t new_index = 8 * index + 1 + nb_child;

  assert(new_index < data_.size());
  return new_index;
}

bool
allrgb::Octree::delete_leaf(const size_t index)
{
  assert(index < data_.size() && index >= data_.size() - nb_leaves_);

  for (size_t i = index; i > 0; i = (i - 1) / 8)
  {
    if (at_(i) <= 0)
      return false;
    at_(i) -= 1;
  }
  if (at_(0) <= 0)
    return false;
  at_(0) -= 1;
  return true;
}

size_t&
allrgb::Octree::at_(const size_t index)
{
  assert(index <= data_.size());

  return data_.at(index);
}

size_t&
allrgb::Octree::operator[](const size_t index)
{
  return data_[index];
}

void
allrgb::Octree::init_()
{
  init_(0, nb_leaves_);
}

void
allrgb::Octree::init_(size_t index, size_t sub_leaves)
{
  data_.at(index) = sub_leaves;

  if (sub_leaves <= 1)
    return;

  for (size_t i = 0; i < 8; ++i)
    init_(8 * index + 1 + i, sub_leaves / 8);
}
