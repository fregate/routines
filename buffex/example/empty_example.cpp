#include <buffex/buffex.hpp>

#include <cstdint>
#include <vector>

#include <iostream>

constexpr size_t chunk = 1024;

auto main() -> int
{
  std::vector<uint8_t> data;
  data.reserve(chunk * 5);
  const auto * d = data.data();

  for (uint8_t i = 1; i <= 5; ++i) {
    data[(i - 1) * chunk] = i;
  }

  BufferEx b;

  b.Write((const char*)data.data(), chunk);
  std::cout << "#1 size: " << b.Size() << std::endl;

  b.Write((const char*)data.data() + chunk, chunk / 3);
  std::cout << "#2 size: " << b.Size() << std::endl;

  std::cout << "#2 cmp: " << memcmp(data.data(), b.Data(), chunk + chunk / 3) << std::endl;

  b.Reset();
  b.Write((const char*)data.data(), chunk / 3);
  std::cout << "#3 size: " << b.Size() << std::endl;

  b.Write((const char*)data.data() + chunk / 3, chunk * 3);
  std::cout << "#4 size: " << b.Size() << std::endl;

  std::cout << "#4 cmp: " << memcmp(data.data(), b.Data(), chunk * 3 + chunk / 3) << std::endl;

  return 0;
}
