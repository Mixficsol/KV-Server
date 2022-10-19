#include "redis.h"
#include <iostream>

using namespace std;
int main()
{
      Redis *r = new Redis();
        if(!r->connect("192.0.0.1", 6379))
              {
                      printf("connect error!\n");
                          return 0;
                            }
          r->auth("123456");
            r->set("name", "Andy");
              printf("Get the name is %s\n", r->get("name").c_str());
              cout << "ehhl" << endl;
                delete r;
                  return 0;
}


