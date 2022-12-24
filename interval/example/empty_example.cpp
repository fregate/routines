#include <interval/interval.hpp>

#include <iostream>

void IntervalMapTest()
{
  std::cout << 1 << ". --- (empty) => --- (empty)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(1, 10, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 0\n";
  }

  std::cout << "\n";
  std::cout << 2 << ". -B- => -BA- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(1, 10, 'B');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'B') (10, 'A') \n";
  }

  std::cout << "\n";
  std::cout <<         "     |--A---| (default value)\n";
  std::cout << 3 << ". -----BBB---- => --- (empty)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(3, 10, 'B');
    imap.assign(1, 100, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 0\n";
  }

  std::cout << "\n";
  std::cout << 4 << ". --- => --BA--BA- (4)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(1, 5, 'B');
    imap.assign(10, 20, 'B');
    std::cout << "map size: " << imap.m_map.size() << " expect: 4\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'B')(5, 'A') (10, 'B')(20, 'A') \n";
  }

  std::cout << "\n";
  std::cout <<         "    |-B-| (default value)\n";
  std::cout << 5 << ". -BA---- => -B___A- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(1, 2, 'B');
    imap.assign(2, 5, 'B');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'B')(5, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |A| (default value)\n";
  std::cout << 6 << ". -B_____BA---- => -BABA- (4)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(1, 20, 'B');
    imap.assign(4, 5, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 4\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'B')(4, 'A') (5, 'B')(20, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |A-----| (default value)\n";
  std::cout << 7 << ". -B_____A---- => -BA- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(1, 5, 'B');
    imap.assign(3, 10, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'B')(3, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "        |A| (default value)\n";
  std::cout << 8 << ". -B_____A---- => -B__A- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(1, 5, 'B');
    imap.assign(4, 5, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'B')(4, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "    |--B-| (default value)\n";
  std::cout << 9 << ". ----B___A---- => -B__A- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(2, 7, 'B');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (2, 'B')(10, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "     |--A-| (default value)\n";
  std::cout << 10 << ". ----B___A---- => -B_A- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(2, 7, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (7, 'B')(10, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "               |--A-| (default value)\n";
  std::cout << 11 << ". ----B___A---------- => -B__A- (2, nothing changes)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(20, 30, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (5, 'B')(10, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |--A-| (default value)\n";
  std::cout << 12 << ". -----------B___A-- => -B__A- (2, nothing changes)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(20, 30, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (5, 'B')(10, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |C|\n";
  std::cout << 13 << ". -B___A-- => -BCA- (3)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(7, 10, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 3\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (5, 'B')(7, 'C')(10, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |C---|\n";
  std::cout << 14 << ". -B___A-- => -BC___A- (3)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(7, 20, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 3\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (5, 'B')(7, 'C')(20, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "     |---C---|\n";
  std::cout << 15 << ". ---B___A--- => -C___A- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(1, 20, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'C')(20, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "       |C|\n";
  std::cout << 16 << ". -B_______A--- => -BCBA- (4)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 20, 'B');
    imap.assign(9, 13, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 4\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (5, 'B')(9, 'C')(13, 'B')(20, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |--C  |\n";
  std::cout << 17 << ". ----B_______A- => -CBA- (3)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 20, 'B');
    imap.assign(1, 13, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 3\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (1, 'C')(13, 'B')(20, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "          |--D  |\n";
  std::cout << 18 << ". ----B__C_____A- => -BDA- (3)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 20, 'B');
    imap.assign(9, 13, 'C');
    imap.assign(7, 20, 'D');
    std::cout << "map size: " << imap.m_map.size() << " expect: 3\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (5, 'B')(7, 'D')(20, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |C|\n";
  std::cout << 19 << ". --------BA- => -CA---BA- (4)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(7, 8, 'B');
    imap.assign(2, 3, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 4\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (2, 'C')(3, 'A')(7, 'B')(8, 'A')\n";
  }

  std::cout << "\n";
  std::cout << "assign empty range\n";
  std::cout << 20 << ". -BA- => -BA- (2, nothig changes)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(7, 8, 'B');
    imap.assign(9, 9, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (7, 'B')(8, 'A')\n";
  }

  std::cout << "\n";
  std::cout << "assign invalid range\n";
  std::cout << 21 << ". -BA- => -BA- (2, nothig changes)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(7, 8, 'B');
    imap.assign(19, 9, 'C');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (7, 'B')(8, 'A')\n";
  }

  std::cout << "\n";
  std::cout <<     "      |A| (default value)\n";
  std::cout << 21 << ". ----B___A---- => -B_A- (2)\n";
  {
    interval_map<int, char> imap('A');
    imap.assign(5, 10, 'B');
    imap.assign(2, 5, 'A');
    std::cout << "map size: " << imap.m_map.size() << " expect: 2\n";
    for (const auto& [key, val] : imap.m_map) {
      std::cout << "key: " << key << " val: " << val << "\n";
    }
    std::cout << "expect: (5, 'B')(10, 'A')\n";
  }
}

auto main() -> int
{
  IntervalMapTest();
  return 0;
}
