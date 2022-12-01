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

  std::cout << std::boolalpha << "check 9 " << ar.check(9) <<  std::endl;
  std::cout << std::boolalpha << "check 0 " << ar.check(0) << std::endl;
  std::cout << std::boolalpha << "check 5 " << ar.check(5) <<  std::endl;
  std::cout << std::boolalpha << "check 4 " << ar.check(4) <<  std::endl;

  return 0;
}
