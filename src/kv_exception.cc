#include "kv_exception.h"

#include <glog/logging.h>

bool FDException(int fd) {
  if (fd < 0 ) {
    return true;
  } else {
    return false;
  }
}

bool IOException(int n) {
  if (n < 0) {
    return true;
  } else {
    return false;
  }
}
