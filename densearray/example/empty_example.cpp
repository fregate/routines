#include <densearray/densearray.hpp>

#include <iostream>

auto main() -> int
{
  densearray::DenseArray ar;

  std::cout << "hole: " << ar.get_hole() << std::endl;

  ar.insert(5);
  ar.insert(2);
  ar.insert(1);

  std::cout << "hole: " << ar.get_hole() << std::endl;

  ar.insert(0);

  std::cout << "hole: " << ar.get_hole() << std::endl;

  ar.insert(1);

  return 0;
}
