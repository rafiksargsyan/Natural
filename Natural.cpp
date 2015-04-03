#include "Natural.h"
#include <string>
#include <cstring>

void Natural::setZero() {
  length = 1;
  n = new Block[1];
  n[0] = 0;
}

void Natural::initWithBlock(Block u) {
  if ( u == 0 ) {
    setZero();
  } else {
    Block m[2] = {0};
    int i = 1;
    while ( u != 0 ) {
      m[i--] = u % BASE;
      u /= BASE;
    }
    length = 1 - i;
    n = new Block[length]; 
    for (int j = 0; j < length; ++j) {
      n[j] = m[i+1+j];
    } 
  }
}   

Natural::Natural(Block u) {
  initWithBlock(u);
}

Natural::Natural(int u) {
  if ( u < 0 ) {
    u = 0;
    initWithBlock(static_cast<Block>(u));
    return;
  }
  initWithBlock(static_cast<Block>(u));
}

Natural::Natural(const char* m, const size_t length) {
  size_t l = ( length <= strlen(m) ) ? length : strlen(m);
  if ( l == 0 ) {
    setZero();
  } else {
    for (size_t i = 0; i < l; ++i) {
      if ( m[i] < '0' || m[i] > '9' ) {
        setZero();
        return;
      }
    }
    size_t i = 0;
    if ( m[0] == '0' ) {
      while ( i < l && m[i] == '0' ) ++i;
    }
    if ( i == l ) {
      setZero();
      return;
    }
    size_t j = l - 1;
    this->length = ( j - i + EXPONENT ) / EXPONENT;
    n = new Block[this->length];
    size_t k = this->length - 1;
    while ( j > i + EXPONENT - 1 ) {
      n[k--] = stringToUInt(m, j - EXPONENT + 1, j);
      j -= EXPONENT;
    }
    n[k] = stringToUInt(m, i, j);
  }
}

Natural::Natural(const Natural& m) {
  length = m.length;
  n = new Block[length];
  for (size_t i = 0; i < length; ++i) {
    n[i] = m.n[i];
  }
}

Natural::~Natural() {
  delete[] n;
}

Natural& Natural::operator=(const Natural& m) {
  if ( this != &m ) {
    delete[] n;
    length = m.length;
    n = new Block[length];
    for (size_t i = 0; i < length; ++i) {
      n[i] = m.n[i];
    }
  }
  return *this;
}
 
Natural& Natural::operator+=(const Natural& m) { 
  Block* sum = NULL;
  if ( length >= m.length ) {
    size_t sum_length = length + 1;
    sum = new Block[sum_length];
    Block carry = 0;
    size_t i = m.length, j = length, k = sum_length;
    for (; i > 0; --i, --j, --k) {
      Block temp = n[j-1] + m.n[i-1] + carry;
      sum[k-1] = temp % BASE;
      carry = temp / BASE;
    }
    for (; j > 0; --j, --k) {
      Block temp = n[j-1] + carry;
      sum[k-1] = temp % BASE;
      carry = temp / BASE;
    }
    sum[0] = carry;
    i = carry ? 0 : 1;
    length = carry ? sum_length : sum_length - 1;
    delete[] n;
    n = new Block[length];
    for (j = 0; j < length; ++j, ++i) {
      n[j] = sum[i];
    }
    delete[] sum;
    return *this;
  } else {
    size_t sum_length = m.length + 1;
    sum = new Block[sum_length];
    Block carry = 0;
    size_t i = length, j = m.length, k = sum_length;
    for (; i > 0; --i, --j, --k) {
      Block temp = m.n[j-1] + n[i-1] + carry;
      sum[k-1] = temp % BASE;
      carry = temp / BASE;
    }
    for (; j > 0; --j, --k) {
      Block temp = m.n[j-1] + carry;
      sum[k-1] = temp % BASE;
      carry = temp / BASE;
    }
    sum[0] = carry;
    i = carry ? 0 : 1;
    length = carry ? sum_length : sum_length - 1;
    delete[] n;
    n = new Block[length];
    for (j = 0; j < length; ++j, ++i) {
      n[j] = sum[i];
    }
    delete[] sum;
    return *this;
  }
}

Natural& Natural::operator-=(const Natural& m) {
  if ( *this <= m ) {
    delete[] n;
    setZero();
    return *this;
  }
  size_t i = length;
  size_t j = m.length;
  for (; j >= 1; --j, --i) {
    if ( n[i-1] >= m.n[j-1] ) {
      n[i-1] -= m.n[j-1];
    } else {
      size_t k = i - 1;
      while ( n[k-1] == 0 ) --k;
      --n[k-1];
      for (size_t l = k; l <= i - 2; ++l) {
        n[l] = BASE - 1;
      }
      n[i-1] += ( BASE - m.n[j-1] );
    }
  }
  if ( n[0] != 0 ) {
    return *this;
  } else {
    size_t i = 0;
    while ( n[i] == 0 ) ++i; 
    length -= i;
    Block* temp = new Block[length];
    for (size_t j = 0; j < length; ++j) {
      temp[j] = n[j+i];
    }
    delete[] n;
    n = new Block[length];
    for (size_t i = 0; i < length; ++i) {
      n[i] = temp[i];
    }
    delete[] temp;
    return *this;
  }        
}

Natural& Natural::operator*=(const Natural& m) {
  if (*this == Natural() || m == Natural()) {
    return *this = Natural();
  }
  size_t prod_length = length + m.length;
  Block* prod = new Block[prod_length];
  Block* temp = new Block[length + 1];
  for (size_t i = 0; i < prod_length; ++i) {
    prod[i] = 0;
  }
  for (size_t i = m.length; i > 0; --i) {
    Block carry = 0;
    for (size_t j = length; j > 0; --j) {
      TwoBlocks temp1 = (TwoBlocks)m.n[i-1] * (TwoBlocks)n[j-1] + carry;
      temp[j] = temp1 % BASE;
      carry = temp1 / BASE;
    }
    temp[0] = carry;
    carry = 0;
    size_t l = length + 1;
    size_t k = prod_length - (m.length - i);
    for (; l > 0; --k, --l) {
      Block temp2 = prod[k-1] + temp[l-1] + carry;
      prod[k - 1] = temp2 % BASE;
      carry = temp2 / BASE;
    }
    if ( carry ) {
      prod[k-1] = carry;
    }
  }
  size_t i = prod[0] ? 0 : 1;
  length = prod[0] ? prod_length : prod_length - 1;
  delete[] n;
  delete[] temp;
  n = new Block[length];
  for (size_t j = 0; j < length; ++j, ++i) {
    n[j] = prod[i];
  }
  delete[] prod;
  return *this;
}

void Natural::initWith(const Natural& source, size_t from, size_t to) {
  delete[] n;
  while ( source.n[from] == 0 && from != to ) ++from;
  length = to - from + 1;
  n = new Block[length];
  for (size_t i = 0; i < length; ++i) {
    n[i] = source.n[from + i];
  }
}

void Natural::addBlock(const Block& b) {
  if ( *this == 0 ) {
    n[0] = b;
    return;
  }
  Block* temp = new Block[length];
  for ( size_t i = 0; i < length; ++i ) {
    temp[i] = n[i];
  }
  delete[] n;
  ++length;
  n = new Block[length];
  for ( size_t i = 0; i < length - 1; ++i ) {
    n[i] = temp[i];
  }
  delete[] temp;
  n[length - 1] = b;
} 

size_t Natural::log(const Block& b) {
  Block temp = 10;
  size_t count = 0;
  while ( temp <= b ) {
    temp *= 10;
    ++count;
  }
  return count;
}

Block Natural::divisionHelper(Natural a, const Natural& b, Natural& remainder) {
  size_t l1;
  size_t l2;
  size_t orderDif;
  if ( a.length == b.length ) {
    l1 = log(a.n[0]);
    l2 = log(b.n[0]);
    if ( l1 < l2 ) {
      return 0;
      remainder = a;
    }
    orderDif = l1 - l2;
  } else {
    orderDif = EXPONENT;
  }
  Natural quotient = 0;
  for (size_t i = orderDif + 1; i >= 1 ; --i) {
    Natural temp2 = tenTo(i - 1);
    Natural temp3 = b * temp2;
    Natural temp4 = 0;
    while ( temp4 + temp3 <= a ) {
      temp4 += temp3;
      quotient += temp2;
    }
    a -= temp4;
  }
  remainder = a;
  return quotient.n[0];       
}

Natural& Natural::operator/=(const Natural& m) {
  if ( m == 0 ) throw DivisionByZero();
  if ( *this < m ) {
    delete[] n;
    setZero();
    return *this;
  }
  size_t lengthTemp = length - m.length + 1;
  Block* quotientTemp = new Block[lengthTemp];
  Natural temp;
  temp.initWith(*this, 0, m.length - 1);
  size_t nextBlockIndex = m.length;
  size_t i = 0;
  quotientTemp[i++] = divisionHelper(temp, m, temp);
  while ( nextBlockIndex < length ) {
    temp.addBlock(n[nextBlockIndex++]);
    quotientTemp[i++] = divisionHelper(temp, m, temp); 
  }
  delete[] n;
  if ( quotientTemp[0] ) {
    length = lengthTemp;
    n = quotientTemp;
    return *this;
  }  
  length = lengthTemp - 1;
  n = new Block[length]; 
  for (size_t i = 0; i < length; ++i) {
    n[i] = quotientTemp[i + 1];
  }
  return *this;
}

Natural& Natural::operator%=(const Natural& m) {
    *this = (*this) - ( (*this) / m ) * m;
    return *this;
}

Natural& Natural::operator++() {
  this->operator+=(Natural(1));
  return *this;
}

Natural Natural::operator++(int a) {
  Natural tmp(*this);
  this->operator+=(Natural(1));
  return tmp;
}

Natural& Natural::operator--() {
  this->operator-=(Natural(1));
  return *this;
}

Natural Natural::operator--(int a) {
  Natural tmp(*this);
  this->operator-=(Natural(1));
  return tmp;
}

Natural Natural::operator%(const Natural& m) const {
  Natural temp = *this;
  return temp %= m;
}

Natural Natural::operator/(const Natural& m) const {
  Natural temp = *this;
  return temp /= m;
}

Natural Natural::operator+(const Natural& m) const {
  Natural temp = *this;
  return temp += m;
}

Natural Natural::operator-(const Natural& m) const {
  Natural temp = *this;
  return temp -= m;
}

Natural Natural::operator*(const Natural& m) const {
  Natural temp = *this;
  return temp *= m;
}

Natural Natural::tenTo(size_t exp) {
  Natural ten_exp(1);
  for (size_t i = 1; i <= exp; ++i) {
    ten_exp *= Natural(10);
  }
  return ten_exp;
}

Block Natural::tenTo(int exp) {
  Block p = 1;
  for (int i = 0; i < exp; ++i) {
    p *= 10;
  }
  return p;
}

Block Natural::stringToUInt(const char* m, int i, int j) {
  Block uintNumber = 0;
  for (int k = 0; k <= j - i; ++k) {
    uintNumber += ( m[j-k] & 0xf ) * tenTo(k);
  }
  return uintNumber;
}

Block Natural::stringToUInt(std::string m, int i, int j) {
  Block uintNumber = 0;
  for (int k = 0; k <= j - i; ++k) {
    uintNumber += ( m[j-k] & 0xf ) * tenTo(k);
  }
  return uintNumber;
}

std::ostream& operator<<(std::ostream& out, const Natural& n) { 
  out << n.n[0];
  for (size_t i = 1; i < n.length; ++i) {
    for (int j = EXPONENT - 1; j >= 0; --j) {
      out << ( n.n[i] / Natural::tenTo(j) % 10 );
    }
  }
  return out;
}

std::istream& operator>>(std::istream& in, Natural& n)
{
  delete[] n.n;
  std::string m;
  in >> m;
  if ( m.length() == 0 ) {
    n.setZero();
    return in;
  } else {
    for (size_t i = 0; i < m.length(); ++i) {
      if ( m[i] < '0' || m[i] > '9' ) {
        n.setZero();
        return in;
      }
    }
    size_t i = 0;
    if ( m[0] == '0' ) {
      while ( i < m.length() && m[i] == '0' ) ++i;
    }
    if ( i == m.length() ) {
      n.setZero();
      return in;
    }
    size_t j = m.length() - 1;
    n.length = (j - i + EXPONENT) / EXPONENT;
    n.n = new Block[n.length]; 
    size_t k = n.length - 1;
    while ( j > i + EXPONENT - 1 ) {
      n.n[k--] = Natural::stringToUInt(m, j - EXPONENT + 1, j);
      j -= EXPONENT;
    }
    n.n[k] = Natural::stringToUInt(m, i, j);
  }  
  return in;          
}

bool operator==(const Natural& x, const Natural& y) {
  if ( x.length != y.length ) {
    return false;
  }
  for (size_t i = 0; i < x.length; ++i) {
    if ( x.n[i] != y.n[i] ) {
      return false;
    }
  }
  return true;
} 

bool operator!=(const Natural& x, const Natural& y) {
  return !(x == y);
}

bool operator<=(const Natural& x, const Natural& y) {
  if ( x.length < y.length ) {
    return true;
  }
  if ( y.length < x.length ) {
    return false;
  }
  size_t i = 0;
  while ( i < x.length && x.n[i] == y.n[i] )  ++i;
  if ( i == x.length ) 
    return true;
  if ( x.n[i] < y.n[i] )
    return true;
  return false;
}   
        
bool operator>=(const Natural& x, const Natural& y) {
  return y <= x;
}

bool operator<(const Natural& x, const Natural& y) {
  return (x <= y) && (x != y);
}

bool operator>(const Natural& x, const Natural& y) {
  return y < x;
}

