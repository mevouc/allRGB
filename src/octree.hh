#pragma once

# include <vector>

namespace allrgb
{
  class Octree
  {
  public:
    Octree(size_t depth);
    size_t at(const size_t index) const;

    size_t index_child(const size_t index, const size_t nb_child);

  private:
    void init_();
    void init_(size_t index, size_t sub_leaves);

  private:
    std::vector<size_t> data_;
    size_t nb_leaves_;
  };
}
