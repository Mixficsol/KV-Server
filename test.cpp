#include <iostream>
#include <stdio.h>
#include <assert.h>
#include "csapp.h"

using namespace std;

char buf[1010];

int main() {
  Fgets(buf, MAXLINE, stdin);
 /* for (int i = 0; str[i] != '\0'; i++) {
    a = a + str[i];
  }
  cout << "hello" << endl;*/
/*  for (int i = 0; i <= 10; i+=2) {
    str[i] = '1';
    str[i + 1] = '\n';
  }*/
  for (int i = 0; i <= 10; i++) {
    cout << buf[i];  
  }
  cout << endl;
  return 0;
}
