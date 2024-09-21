#include <errno.h>
#include <math.h>
#include <stdio.h>

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

int main(void) {
  double number;
  printf("Enter number: ");
  if (scanf("%lf", &number) != 1) {
    fprintf(stderr, "Bruh in scanf\n");
    return -1;
  }

  double res = mysqrt(number);
  printf("%lf\n", res);

  return 0;
}
