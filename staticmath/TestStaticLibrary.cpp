#include "StaticMath.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
  double a = 10;
  double b = 2;
  cout << "a + b = " << add(a, b) << endl;
  cout << "a - b = " << sub(a, b) << endl;
  cout << "a * b = " << mul(a, b) << endl;
  cout << "a / b = " << div(a, b) << endl;

  return 0;
}

