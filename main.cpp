#include <iostream>
#include "Natural.h"
using namespace std;

Natural factorial(const Natural& n, const Natural& m) {
  if ( n == 0 || m < n ) {
    return 1;
  }
  Natural fact(n);
  for (Natural i = n + 1; i <= m; ++i) {
    fact *= i;
  }
  return fact;
}

Natural tenTo(const Natural& n) {
  if ( n == 0 ) {
     return 1;
  }
  Natural exp(1);
  for (Natural i = 1; i <= n; i++) {
    exp *= 10;
  }
  return exp;
}

Natural ee(Natural n, Natural precision) {
  Natural e(0);
  for (Natural i = 0; i <= n; ++i) {
    e += factorial(i + 1, n);
  }
  e *= tenTo(precision);
  return e / factorial(1, n);
}
  
int main()
{
    Natural m("12341234151");
    Natural n;
    Natural a;
    Natural b;
    /*while (true) { 
        (cin >> m) >> n;
        a = m, b = n;
        cout << m << ' ' << n << '\n';
        cout << "m == n : " << (m == n) << '\n';
        cout << "m != n : " << (m != n) << '\n';
        cout << "m <= n : " << (m <= n) << '\n';
        cout << "m >= n : " << (m >= n) << '\n';
        cout << "m < n : " << (m < n) << '\n';
        cout << "m > n : " << (m > n) << '\n';
        cout << "m - n : " << (m -= n) << '\n';
        cout << "m / n : " << (a /= b) << '\n';
    }*/
    //for (int i = 1; i <= 10000; ++i) {
    //  factorial(1, 1000) / factorial(1, 50);
    //} 
    //cout << factorial(1, 10000);
    cout << ee(1001, 2000) << "\n";
    /*while ( true ) { 
      (cin >> m) >> n;
      cout << m / n << '\n';
    }*/
    return 0;
}
