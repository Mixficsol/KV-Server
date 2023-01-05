#include "kv_exception.h"

bool FD_Exception(int fd) {
  if (fd < 0 ) {
    return true;
  } else {
    return false;
  }
}

bool IO_Exception(int n) {
  if (n < 0) {
    return true;
  } else {
    return false;
  }
}
