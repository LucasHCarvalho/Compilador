#include "tinyexpr.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
const char *c = "3!!";
  double r = te_interp(c, 0);
  printf("The expression:\n\t%s\nevaluates to:\n\t%f\n", c, r);
  return 0;
}
