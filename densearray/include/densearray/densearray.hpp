#pragma once

#include <algorithm>
#include <deque>
#include <limits>
#include <stdexcept>

namespace densearray
{

class Range
{
public:
  using ValType = std::uint32_t;

  Range(ValType start, ValType end) { set(start, end); };
  ~Range() = default;

  constexpr bool contains(ValType val) const { return start_ <= val && val <= end_; }
  constexpr bool intersects_expanded(const Range& range) const {
    return range.contains(start_ - 1) || range.contains(end_ + 1);
  }

  constexpr int compare(ValType val) const {
    if (val < start_) return -1;
    else if (contains(val)) return 0;
    else return 1;
  }

  constexpr ValType start() const { return start_; }
  constexpr ValType end() const  { return end_; }

  void set(ValType start, ValType end) {
    if(start > end)
      throw std::invalid_argument("invalid range");

    start_ = start;
    end_ = end;
  }

private:
  Range() = default;

  ValType start_;
  ValType end_;
};

class DenseArray
{
public:
  using RangeType = Range::ValType;

  DenseArray() = default;
  ~DenseArray() = default;

  void insert(RangeType value) {
    const auto it = find(value);
    if (it != items_.end()) {
      const auto cmp = it->compare(value);
      if (cmp != 0) {
        Range range(value, value);
        if (!it->intersects_expanded(range)) {
          auto n =
              items_.insert(cmp > 0 ? std::next(it) : it, std::move(range));
          merge(n != items_.begin() ? std::prev(n) : n,
                n,
                n != items_.end() ? std::next(n) : n);
        } else {
          const auto mut_it = items_.erase(it, it);
          mut_it->set(std::min(it->start(), value), std::max(it->end(), value));
        }
      }
    } else {
      items_.emplace_back(value, value);
    }
  }

  RangeType get_hole() const {
    if (items_.empty()) {
      return std::numeric_limits<RangeType>::min();
    }

    if (items_.begin()->start() != std::numeric_limits<RangeType>::min())
      return std::numeric_limits<RangeType>::min();

    return items_.begin()->end() + 1;
  }

  bool check(RangeType val) const {
    if (items_.empty()) {
      return false;
    }

    const auto bnd = find(val);
    return bnd == items_.end() || !bnd->contains(val) ? false : true;
  }

private:
  using Container = std::deque<Range>;
  using ContainerIterator = Container::iterator;
  using ConstContainerIterator = Container::const_iterator;

  ConstContainerIterator find(RangeType val) const
  {
    auto it =
        std::partition_point(items_.begin(),
                         items_.end(),
          [val](const auto& item) -> bool {
            return item.contains(val);
          });

    return it == items_.begin() ? it : std::prev(it);
  }

  void merge(ContainerIterator prev,
             ContainerIterator current,
             ContainerIterator next)
  {
    if (prev != current && prev->intersects_expanded(*current)) {
      current->set(std::min(prev->start(), current->start()),
                   std::max(prev->end(), current->end()));
      items_.erase(prev);
    }

    if (next != items_.end() && next != current && current->intersects_expanded(*next)) {
      current->set(std::min(next->start(), current->start()),
                   std::max(next->end(), current->end()));
      items_.erase(next);
    }
  }

private:
  Container items_;
};

} // namespace densearray
