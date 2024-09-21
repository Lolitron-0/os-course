#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[], char *envp[]) {
  if (!envp) {
    fprintf(stderr, "envp not supported");
    return -1;
  }

  char *envVal = NULL;
  for (int i = 0; envp[i] != NULL; i++) {
    if (strstr(envp[i], "SQRT_NUM")) {
      envVal = strchr(envp[i], '=');
      envVal++;
    }
  }

  if (!envVal) {
    fprintf(stderr, "use SQRT_NUM env var\n");
    return -1;
  }

  double number;
  number = strtod(envVal, NULL);
  double res = mysqrt(number);
  printf("%lf\n", res);

  return 0;
}
