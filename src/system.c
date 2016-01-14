#include <Arduino.h>

void _yield() {
  yield();
}

int _getpid(void) {
  return 1;
}

int _kill(int pid, int sig) {
  return -1;
}
