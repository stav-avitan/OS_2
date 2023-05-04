#include "codecA.h"

void codecA_encode(char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (islower(str[i])) {
      str[i] = toupper(str[i]);
    } else if (isupper(str[i])) {
      str[i] = tolower(str[i]);
    }
  }
}

void codecA_decode(char* str) {
  codecA_encode(str);
}
