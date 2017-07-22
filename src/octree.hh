#pragma once

# include <vector>

namespace allrgb
{
  class Octree
  {
  public:
    Octree(unsigned depth);

  private:
    void init_();
    void init_(unsigned long index, unsigned long sub_leaves);

  private:
    std::vector<unsigned> data_;
    unsigned long nb_leaves_;
  };
}
