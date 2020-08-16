/**
 * @file matmul.cpp
 * @author Arrvindh Shriraman (ashriram@sfu.ca)
 * @brief
 * @version 0.1
 * @date 2019-02-03
 *
 * @copyright Copyright (c) 2019
 *
 * clang -c -emit-llvm vector.cpp
 * ../../build/bin/logger -fn-name=_Z3fooiPiS_ vector.bc -o vector
 * ./vector 8
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void foo(int n, int *a, int *sum)
{
  int k = n;
  for (int i = 0; i < k; i++)
    *sum = *sum + a[i];

  for (int i = 0; i < k; i = i + 2)
    a[i] = 10;
}

int main(int argc, char *argv[])
{

  assert(argc == 2);
  int n = atoi(argv[1]);
  int *a = new int[n];
  int *sum = new int;
  *sum = 10;
  for (int i = 0; i < n; i++)
  {
    a[i] = i;
  }
  foo(n, a, sum);
  printf("%d\n", *sum);

  return 0;
}
