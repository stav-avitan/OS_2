#include "codecB.h"

void codecB_encode(char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = str[i] + 3;
  }
}

void codecB_decode(char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = str[i] - 3;
  }
}