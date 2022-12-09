#include "buffex/buffex.hpp"

auto main() -> int
{
  auto const result = name();

  return result == "buffex" ? 0 : 1;
}
