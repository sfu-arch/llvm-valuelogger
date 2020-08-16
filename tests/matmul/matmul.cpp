/**
 * @file matmul.cpp
 * @author Arrvindh Shriraman (ashriram@sfu.ca)
 * @brief
 * @version 0.1
 * @date 2019-02-03
 *
 * @copyright Copyright (c) 2019
 *
 *
 * clang -c -emit-llvm matmul.cpp
 * ../../build/bin/logger -fn-name=_Z3fooiPiS_S_ matmul.bc -o matmul
 * ./matmul 8
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int foo(int n, int *a, int *b, int *c) {
  int k = n;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      c[i * k + j] = a[i * 2 + j] + b[i * k + j];
      /* for (int k = 0; k < n; k++)
{
c[i * n + j] += a[i * n + k] * b[k * n + j];
} */
      //	printf("%d\t", c[i * n + j]);
    }
    // printf("\n");
  }

  return c[0];
}

int main(int argc, char *argv[]) {

  assert(argc == 2);
  int n = atoi(argv[1]);
  int *a = new int[n * n];
  int *b = new int[n * n];
  int *c = new int[n * n];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      a[i * n + j] = 0xdeadbeef;
      b[i * n + j] = 0xbeefdead;
      c[i * n + j] = 0;
    }
  }

  printf("%d\n", foo(2, a, b, c));
  //  printf("%d\n", foo(2, a, b, c));

  return 0;
}
