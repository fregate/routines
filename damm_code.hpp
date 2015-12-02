#include <iostream>
#include <cassert>

class Damm {
  public:
    Damm() : interim_digit_(0) {}
  
    void push(int digit) {
      static const int quasigroup[10][10] = {
        {0, 3, 1, 7, 5, 9, 8, 6, 4, 2},
        {7, 0, 9, 2, 1, 5, 4, 8, 6, 3},
        {4, 2, 0, 6, 8, 7, 1, 3, 5, 9},
        {1, 7, 5, 0, 9, 8, 3, 4, 2, 6},
        {6, 1, 2, 3, 0, 4, 5, 9, 7, 8},
        {3, 6, 7, 4, 2, 0, 9, 5, 8, 1},
        {5, 8, 6, 9, 7, 2, 0, 1, 3, 4},
        {8, 9, 4, 5, 3, 6, 2, 0, 1, 7},
        {9, 4, 3, 8, 6, 1, 7, 2, 0, 5},
        {2, 5, 8, 1, 4, 3, 6, 7, 9, 0}
      };
      assert(digit >= 0);
      assert(digit <= 9);
      interim_digit_ = quasigroup[interim_digit_][digit];
    }
    
    int check_digit(void) const {
      return interim_digit_;
    }
    
    void clear(void) {
      interim_digit_ = 0;
    }
    
  private:
    int interim_digit_;
};

int main(void)
{
  Damm d;
  d.push(5);
  d.push(7);
  d.push(2);
  int check_digit = d.check_digit();
  std::cout << "Check digit (572) = " << check_digit << ", expected 4\n";
  
  d.clear();
  d.push(5);
  d.push(7);
  d.push(2);
  d.push(4);
  check_digit = d.check_digit();
  std::cout << "Check digit (5724) = " << check_digit << ", expected 0\n";
  
  return 0;
}
