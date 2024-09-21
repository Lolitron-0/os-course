#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 1e-6

double mysqrt(double number) {
  if (number < 0) {
    return NAN;
  }

  double prev = 0;
  double next = number / 2;

  while (fabs(next - prev) >= EPSILON) {
    prev = next;
    next = 0.5 * (prev + (number / prev));
  }

  return next;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./task1.out double\n");
    return -1;
  }

  double number;
	char* endptr;
	number = strtod(argv[1], &endptr);
  if (*endptr != 0) {
    fprintf(stderr, "Usage: ./task1.out double\n");
    return -1;
  }
  double res = mysqrt(number);
  printf("%lf\n", res);

  return 0;
}
