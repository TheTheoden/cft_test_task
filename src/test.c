#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int simple_test() {
  const StatData case_1_in_a[2] = {
      {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
      {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}};
  const StatData case_1_in_b[2] = {
      {.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode = 2},
      {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2}};
  /* Ожидаемый результат обработки */
  const StatData case_1_out[3] = {
      {.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2},
      {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2},
      {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3}};

  StoreDump(case_1_in_a, 2, "test_data/simple_in1");
  StoreDump(case_1_in_b, 2, "test_data/simple_in2");

  system("./process test_data/simple_in1 test_data/simple_in2 "
         "test_data/simple_out");

  int case_1_result_cnt;
  StatData *case_1_result =
      LoadDump("test_data/simple_out", &case_1_result_cnt);

  if (!case_1_result) {
    fprintf(stderr, "Failed to load the data\n");
    return 1;
  }

  if (case_1_result_cnt != 3) {
    free(case_1_result);
    fprintf(stderr, "Wrong number of insatnces\n");
    return 1;
  }

  for (int i = 0; i < case_1_result_cnt; i++) {
    if (!CompareData(case_1_out[i], case_1_result[i])) {
      free(case_1_result);
      fprintf(stderr, "Results do not match\n");
      return 1;
    }
  }

  free(case_1_result);
  printf("simple_test passed\n");
  return 0;
}

void max_test() {}

int main() {
  srand(time(NULL));
  int num_tests = 0;
  num_tests += simple_test();

  if (num_tests == 0) {
    printf("All tests passed.\n");
  } else {
    printf("Some tests have failed.\n");
  }
  return 0;
}