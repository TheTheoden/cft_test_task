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
      fprintf(stderr, "Data does not match\n");
      return 1;
    }
  }

  free(case_1_result);
  printf("simple_test passed\n");
  return 0;
}

int max_test() {
  StatData *a;
  StatData *b;
  StatData *c;
  StatData *out;
  int a_size, b_size, c_size, out_size;

  a_size = 1e5;
  a = malloc(sizeof(StatData) * a_size);

  if (!a) {
    fprintf(stderr, "Failed to allocate first input\n");
    goto ERROR;
  }

  for (int i = 0; i < a_size; i++) {
    a[i].id = 1;
    a[i].count = 1;
    a[i].cost = 1.0;
    a[i].primary = 1;
    a[i].mode = 0;
  }

  b_size = 1e5;
  b = malloc(sizeof(StatData) * b_size);

  if (!b) {
    fprintf(stderr, "Failed to allocate second input\n");
    goto ERROR;
  }

  for (int i = 0; i < b_size; i++) {
    b[i].id = 2 + i;
    b[i].count = 1;
    b[i].cost = 1.0;
    b[i].primary = 0;
    b[i].mode = (i % 8);
  }

  c_size = 1e5 + 1;
  c = malloc(sizeof(StatData) * c_size);

  if (!c) {
    fprintf(stderr, "Failed to allocate output array\n");
    goto ERROR;
  }

  for (int i = 1; i < c_size; i++) {
    c[i].id = i + 1;
    c[i].count = 1;
    c[i].cost = 1.0;
    c[i].primary = 0;
    c[i].mode = (i - 1) % 8;
  }
  c[0].id = 1;
  c[0].count = 1e5;
  c[0].cost = 100000.0;
  c[0].primary = 1;
  c[0].mode = 0;

  StoreDump(a, a_size, "test_data/max_in1");
  StoreDump(b, b_size, "test_data/max_in2");

  system("./process test_data/max_in1 test_data/max_in2 test_data/max_out");

  out = LoadDump("test_data/max_out", &out_size);

  if (!out) {
    fprintf(stderr, "Failed to load processed data\n");
    goto ERROR;
  }

  if (out_size != c_size) {
    fprintf(stderr, "Wrong number of insatnces\n");
    goto ERROR;
  }

  for (int i = 0; i < c_size; i++) {
    if (!CompareData(out[i], c[i])) {
      fprintf(stderr, "Data does not match\n");
      goto ERROR;
    }
  }

  printf("max_test passed\n");
  free(a);
  free(b);
  free(c);
  free(out);
  return 0;

ERROR:
  if (a)
    free(a);
  if (b)
    free(b);
  if (c)
    free(c);
  if (out)
    free(out);

  return 1;
}

int min_test() {
  StatData *a = NULL;
  StatData *b = NULL;
  StatData *out;
  int a_size, b_size, out_size;

  a_size = 0;
  b_size = 0;

  StoreDump(a, a_size, "test_data/min_in1");
  StoreDump(b, b_size, "test_data/min_in2");

  system("./process test_data/min_in1 test_data/min_in2 test_data/min_out");

  out = LoadDump("test_data/min_out", &out_size);

  if (out_size != 0) {
    if (out)
      free(out);
    fprintf(stderr, "Wrong number of insatnces\n");
    return 1;
  }

  if (out) {
    free(out);
  }

  printf("min_test passed\n");
  return 0;
}

int main() {
  srand(time(NULL));
  int num_tests = 0;
  num_tests += simple_test();
  num_tests += max_test();
  num_tests += min_test();

  if (num_tests == 0) {
    printf("All tests passed.\n");
  } else {
    printf("%d of the tests have failed.\n", num_tests);
  }
  return 0;
}