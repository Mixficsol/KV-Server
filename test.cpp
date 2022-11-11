#include <iostream>
#include "glog/logging.h"

using namespace std;

int main(int argc, char*argv[]) {
  int num_cookies = 11;
  google::InitGoogleLogging(argv[0]); 
  FLAGS_log_dir = "./log";
  LOG_IF(ERROR, num_cookies > 10) << "Got lots of cookies";
  google::ShutdownGoogleLogging();
  return 0;
}
