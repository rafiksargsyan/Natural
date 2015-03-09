#include <iostream>
#include "Natural.h"
#include <string>
using namespace std;

Natural factorial(const Natural& n)
{
    Natural fact(1);
    //long long fact = 1;
    for (long i = 2; i <= n; ++i) {
         fact *= i;
    }
    return fact;
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
        cout << "m \% n : " << (a %= b) << '\n';
    }*/
    cout << factorial(10000);
    return 0;
}
