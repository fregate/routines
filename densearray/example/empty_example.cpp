#include <iostream>

#include <densearray/densearray.hpp>

auto main() -> int
{
  densearray::DenseArray ar;

  std::cout << "hole: " << ar.get_hole() << std::endl;

  // std::vector<int> v = {3, 4, -1, 1}; // 2
  // std::vector<int> v = {7, 8, 9, 11, 12}; // 1

  std::vector<int> v = {43, 20, 39, -7, -8, -2, 8,  17, 10, 17, 12, 6,  37, 17,
                        50, 44, 3,  11, 18, -4, 44, 37, 28, 50, 15, 50, 19, 0,
                        45, 5,  37, 35, 35, 21, 39, 35, 27, -8, -1, 47, 19, 22,
                        1,  1,  47, -4, -7, -3, 16, 21, 2,  7,  6};

  for (auto n : v) {
    if (n < 1)
      continue;

    ar.insert(n);
  }

  std::cout << "hole: " << ar.get_hole() << std::endl;

  return 0;
}
