#ifndef NATURAL
#define NATURAL

#include <ostream>
#include <istream>
#include <limits>
#include <string>

#define BASE 1000000000
#define EXPONENT 9
typedef uint32_t Block;
typedef uint64_t TwoBlocks;

class Natural {
 public:
  Natural(Block = 0);
  Natural(const char*, const size_t = std::numeric_limits<size_t>::max());
  Natural(const Natural&);
  ~Natural();
  Natural& operator=(const Natural&);
  Natural& operator+=(const Natural&);
  Natural& operator-=(const Natural&);
  Natural& operator*=(const Natural&);
  Natural& operator/=(const Natural&);
  Natural& operator%=(const Natural&);
  Natural operator+(const Natural&);
  Natural operator-(const Natural&);
  Natural operator*(const Natural&);
  Natural operator/(const Natural&); 
  Natural operator%(const Natural&);
  friend bool operator==(const Natural&, const Natural&);
  friend bool operator!=(const Natural&, const Natural&);
  friend bool operator<=(const Natural&, const Natural&);
  friend bool operator>=(const Natural&, const Natural&);
  friend bool operator<(const Natural&, const Natural&);
  friend bool operator>(const Natural&, const Natural&);
  friend std::istream& operator>>(std::istream&, Natural&);
  friend std::ostream& operator<<(std::ostream&, const Natural&);
 
 private:
  class DivisionByZero {}; //Exception
  void setZero();
  static Natural tenTo(size_t);
  static Block tenTo(int);
  static Block stringToUInt(const char*, int, int);
  static Block stringToUInt(std::string, int, int);
  Block* n;
  size_t length;
};

#endif //NATURAL
