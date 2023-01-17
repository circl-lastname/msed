#include <stdio.h>

int main() {
  fputs("#include <stdint.h>\nuint8_t font_pixels[]={", stdout);
  
  int ch;
  while ((ch = getchar()) != EOF) {
    printf("%i,", ch);
  }
  
  fputs("};", stdout);
  
  return 0;
}
