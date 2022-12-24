#pragma once

#include <map>
template<typename K, typename V>
class interval_map
{
  friend void IntervalMapTest();

  V m_valBegin;
  std::map<K, V> m_map;

public:
  // constructor associates whole range of K with val
  interval_map(V const& val)
      : m_valBegin(val)
  {
  }

  // Assign value val to interval [keyBegin, keyEnd).
  // Overwrite previous values in this interval.
  // Conforming to the C++ Standard Library conventions, the interval
  // includes keyBegin, but excludes keyEnd.
  // If !( keyBegin < keyEnd ), this designates an empty interval,
  // and assign must do nothing.
  void assign(K const& keyBegin, K const& keyEnd, V const& val)
  {
    if (!(keyBegin < keyEnd))
          return;

    if (m_map.empty() && val == m_valBegin)
      return;

    if (m_map.empty())
    {
      m_map.emplace(keyBegin, val);
      m_map.emplace(keyEnd, m_valBegin);
      return;
    }

    auto left = m_map.lower_bound(keyBegin);
    auto right = m_map.upper_bound(keyEnd);

    std::optional<V> insertion;
    std::optional<V> stopper = (right == m_map.end() && val != m_valBegin)
            ? std::make_optional(m_valBegin)
            : std::nullopt;

    if (right != m_map.end())
    {
      if (right != m_map.begin() && val != std::prev(right)->second)
      {
        stopper = std::prev(right)->second;
      }
      else if (right == m_map.begin())
      {
        stopper = m_valBegin;
      }
    }

    right = m_map.erase(left, right);
    if (m_map.empty() && val != m_valBegin)
    {
      insertion = val;
    }
    else if (right == m_map.end())
    {
      if (m_map.empty() && val != m_valBegin)
      {
        insertion = val;
      }
      else if (!m_map.empty() && std::prev(right)->second != val)
      {
        insertion = val;
      }
    }
    else if (right != m_map.end())
    {
      if (val == m_valBegin && right == m_map.begin())
      {
        insertion = std::nullopt;
      }
      else
      {
        insertion = val;
      }
    }

    if (insertion)
      m_map.emplace(keyBegin, *insertion);

    if (stopper)
      m_map.emplace(keyEnd, *stopper);
  }

  // look-up of the value associated with key
  V const& operator[](K const& key) const
  {
    auto it = m_map.upper_bound(key);
    if (it == m_map.begin()) {
      return m_valBegin;
    } else {
      return (--it)->second;
    }
  }
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.
