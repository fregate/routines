#include <cstring>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include <iostream>

#include "MurmurHash3.h"

namespace mur {

#define FORCE_INLINE inline __attribute__((always_inline))

inline uint64_t rotl64(uint64_t x, int8_t r) {
  return (x << r) | (x >> (64 - r));
}

#define ROTL64(x, y) mur::rotl64(x, y)

#define BIG_CONSTANT(x) (x##LLU)

FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}

} // namespace mur

using u128_t = std::tuple<std::uint64_t, std::uint64_t>;
using raw128 = std::uint64_t[2];

template <class T>
concept has_size = requires(T v) { v.size(); };

template <class T>
concept has_data = requires(T v) { v.data(); };

template <class T>
concept has_value_type = requires { typename T::value_type; };

template <class T>
concept Container = has_value_type<T> && has_size<T> && has_data<T>;

class MurCtx {
public:
  MurCtx(std::uint32_t seed)
  : seed_{seed}
  , data_size_{0}
  , buffer_{0}
  , index_{0}
  , offset_{0}
  {}

  u128_t build() {
    constexpr uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    constexpr uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

    std::uint64_t h1{seed_};
    std::uint64_t h2{seed_};

    index_ = 0;
    offset_ = 0;

    const std::size_t nblocks = data_size_ / 16;
    for(std::size_t i = 0; i < nblocks; i++)
    {
      auto [k1, k2] = getblocks(i);

      k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

      h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

      k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

      h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
    }

    const auto data_tail = tail();

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    if ((data_size_ & 15) != data_tail.size())
    {
      std::cout << "wrong tail\n";
      exit(1);
    }

    switch(data_size_ & 15)
    {
    case 15: k2 ^= ((uint64_t)data_tail[14]) << 48;
    case 14: k2 ^= ((uint64_t)data_tail[13]) << 40;
    case 13: k2 ^= ((uint64_t)data_tail[12]) << 32;
    case 12: k2 ^= ((uint64_t)data_tail[11]) << 24;
    case 11: k2 ^= ((uint64_t)data_tail[10]) << 16;
    case 10: k2 ^= ((uint64_t)data_tail[ 9]) << 8;
    case  9: k2 ^= ((uint64_t)data_tail[ 8]) << 0;
            k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

    case  8: k1 ^= ((uint64_t)data_tail[ 7]) << 56;
    case  7: k1 ^= ((uint64_t)data_tail[ 6]) << 48;
    case  6: k1 ^= ((uint64_t)data_tail[ 5]) << 40;
    case  5: k1 ^= ((uint64_t)data_tail[ 4]) << 32;
    case  4: k1 ^= ((uint64_t)data_tail[ 3]) << 24;
    case  3: k1 ^= ((uint64_t)data_tail[ 2]) << 16;
    case  2: k1 ^= ((uint64_t)data_tail[ 1]) << 8;
    case  1: k1 ^= ((uint64_t)data_tail[ 0]) << 0;
            k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
    };

    h1 ^= data_size_; h2 ^= data_size_;

    h1 += h2;
    h2 += h1;

    h1 = mur::fmix64(h1);
    h2 = mur::fmix64(h2);

    h1 += h2;
    h2 += h1;

    return {h1, h2};
  }

  template <typename N>
    requires std::is_arithmetic_v<N>
  MurCtx & hash(const N & num) {
    overlaps_.push_back(std::span<const std::uint8_t>{
        reinterpret_cast<const std::uint8_t *>(&num), sizeof(N)});
    data_size_ += sizeof(N);
    return *this;
  }

  template <Container T>
  MurCtx & hash(const T & data) {
    overlaps_.push_back(std::span<const std::uint8_t>{
        reinterpret_cast<const std::uint8_t *>(data.data()), data.size()});
    data_size_ += data.size();
    return *this;
  }

private:
  u128_t getblocks(std::size_t block_index)
  {
    constexpr std::size_t chunk_size = 16;
    std::size_t current_chunk_size{chunk_size};
    while (current_chunk_size != 0)
    {
      const auto & ov = overlaps_[index_];
      const auto os = ov.size() - offset_;
      if (os > current_chunk_size) {
        std::memcpy(&buffer_.raw[0] + chunk_size - current_chunk_size, &*(ov.begin() + offset_), current_chunk_size);
        offset_ += current_chunk_size;
        return {buffer_.block_[0], buffer_.block_[1]};
      } else {
        std::memcpy(&buffer_.raw[0] + chunk_size - current_chunk_size, &*(ov.data() + offset_), os);
        current_chunk_size -= os;
        offset_ = 0;
        index_ += 1;
      }
    }
    
    return {buffer_.block_[0], buffer_.block_[1]};
  }

  std::span<const std::uint8_t> tail() const
  {
    if (index_ >= overlaps_.size())
      return {};

    return overlaps_.back().subspan(offset_);
  }

  union d128
  {
    std::uint8_t raw[16];
    std::uint64_t block_[2];
  };

  std::uint32_t seed_;
  std::size_t data_size_;
  d128 buffer_;
  std::size_t index_;
  std::size_t offset_;
  std::vector<std::span<const std::uint8_t>> overlaps_;
};

constexpr std::string str_data{"0123456789"};
constexpr std::uint8_t u8{123};
constexpr std::uint16_t u16{65533};
constexpr std::uint64_t u64{1};
constexpr double f64{3.1415926};
constexpr float f32{2.71827};

int main() {
  constexpr std::uint32_t seed{12345};

  const auto hash_all = MurCtx{seed}
                        .hash(u8)
                        .hash(f64)
                        .hash(u16)
                        .hash(str_data)
                        .hash(u64)
                        .hash(f32)
                        .build();

  std::cout << "hash_all\n" << std::hex << std::get<0>(hash_all) << " " << std::get<1>(hash_all) << "\n";
  constexpr auto data_size = sizeof(u8) + str_data.size() + sizeof(u16) + sizeof(u64) + sizeof(f64) + sizeof(f32);
  auto * all = new std::uint8_t[data_size];
  std::size_t offset{0};
  std::memcpy(all + offset, &u8, sizeof(u8)); offset += sizeof(u8);
  std::memcpy(all + offset, &f64, sizeof(f64)); offset += sizeof(f64);
  std::memcpy(all + offset, &u16, sizeof(u16)); offset += sizeof(u16);
  std::memcpy(all + offset, str_data.data(), str_data.size()); offset += str_data.size();
  std::memcpy(all + offset, &u64, sizeof(u64)); offset += sizeof(u64);
  std::memcpy(all + offset, &f32, sizeof(f32)); offset += sizeof(f32);
  raw128 mur;
  MurmurHash3_x64_128(all, data_size, seed, mur);
  std::cout << std::hex << mur[0] << " " << mur[1] << "\n";
  delete [] all;

  {
    const auto chained_hash = MurCtx{seed}.hash(u8).build();
    raw128 mur;
    MurmurHash3_x64_128(&u8, sizeof(u8), seed, mur);
    std::cout << "check u8\n" << std::hex << std::get<0>(chained_hash) << " " << std::get<1>(chained_hash) << "\n";
    std::cout << std::hex << mur[0] << " " << mur[1] << "\n";
  }

  {
    const auto chained_hash = MurCtx{seed}.hash(u16).build();
    raw128 mur;
    MurmurHash3_x64_128(&u16, sizeof(u16), seed, mur);
    std::cout << "check u16\n" << std::hex << std::get<0>(chained_hash) << " " << std::get<1>(chained_hash) << "\n";
    std::cout << std::hex << mur[0] << " " << mur[1] << "\n";
  }

  {
    const auto chained_hash = MurCtx{seed}.hash(u64).build();
    raw128 mur;
    MurmurHash3_x64_128(&u64, sizeof(u64), seed, mur);
    std::cout << "check u64\n" << std::hex << std::get<0>(chained_hash) << " " << std::get<1>(chained_hash) << "\n";
    std::cout << std::hex << mur[0] << " " << mur[1] << "\n";
  }

  {
    const auto chained_hash = MurCtx{seed}.hash(f64).build();
    raw128 mur;
    MurmurHash3_x64_128(&f64, sizeof(f64), seed, mur);
    std::cout << "check f64\n" << std::hex << std::get<0>(chained_hash) << " " << std::get<1>(chained_hash) << "\n";
    std::cout << std::hex << mur[0] << " " << mur[1] << "\n";
  }

  {
    const auto chained_hash = MurCtx{seed}.hash(f32).build();
    raw128 mur;
    MurmurHash3_x64_128(&f32, sizeof(f32), seed, mur);
    std::cout << "check f32\n" << std::hex << std::get<0>(chained_hash) << " " << std::get<1>(chained_hash) << "\n";
    std::cout << std::hex << mur[0] << " " << mur[1] << "\n";
  }

  return 0;
}
