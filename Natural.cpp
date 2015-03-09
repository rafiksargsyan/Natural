#include "Natural.h"
#include <string>
#include <cstring>

void Natural::setZero() {
  length = 1;
  n = new Block[1];
  n[0] = 0;
}

Natural::Natural(Block u) {
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

Natural& Natural::operator*=(const Natural& m)
{
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

Natural& Natural::operator/=(const Natural& m) {
  if ( m == Natural(size_t(0)) ) throw DivisionByZero();
  if ( *this < m ) {
    delete[] n;
    setZero();
    return *this;
  } 
  Natural exp(1);
  Natural ten(10);
  size_t i = 0;
  while ( ( exp * m ) <= *this ) {
    exp *= ten;
    ++i;
  }
  exp = Natural::tenTo(i - 1); 
  size_t j = 2;
  while ( exp * Natural(j) * m <= *this ) ++j;
  exp *= Natural(j - 1);
  for (size_t k = i - 1; k >= 1; --k) {
    Natural exp1 = Natural::tenTo(k - 1);
    size_t j = 1;
    while ( ( Natural(j) * exp1 + exp ) * m <= *this ) ++j;
    exp += Natural(j - 1) * exp1;
  }    
  return (*this) = exp;   
}    

Natural& Natural::operator%=(const Natural& m) {
    *this = (*this) - ( (*this) / m ) * m;
    return *this;
}

Natural Natural::operator%(const Natural& m) {
  Natural temp = *this;
  return temp %= m;
}

Natural Natural::operator/(const Natural& m) {
  Natural temp = *this;
  return temp /= m;
}

Natural Natural::operator+(const Natural& m) {
  Natural temp = *this;
  return temp += m;
}

Natural Natural::operator-(const Natural& m) {
  Natural temp = *this;
  return temp -= m;
}

Natural Natural::operator*(const Natural& m) {
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
    uintNumber += ( m[j-k] & 0xf ) * Natural::tenTo(k);
  }
  return uintNumber;
}

Block Natural::stringToUInt(std::string m, int i, int j) {
  Block uintNumber = 0;
  for (int k = 0; k <= j - i; ++k) {
    uintNumber += ( m[j-k] & 0xf ) * Natural::tenTo(k);
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

