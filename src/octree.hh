#pragma once

# include <vector>

namespace allrgb
{
  class Octree
  {
  public:
    Octree(size_t depth);

    const size_t& at(const size_t index) const;
    const size_t& operator[](const size_t index) const;

    size_t index_child(const size_t index, const size_t nb_child);
    bool delete_leaf(const size_t index);

  private:
    void init_();
    void init_(size_t index, size_t sub_leaves);

    size_t& at_(const size_t index);
    size_t& operator[](const size_t index);

  private:
    std::vector<size_t> data_;
    const size_t nb_leaves_;
  };
}
