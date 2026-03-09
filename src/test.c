#include "common.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define COMPLETE_TEST(test_id, data_a, data_b, data_c)                         \
  RunTest(test_id, data_a, ARRAY_SIZE(data_a), data_b, ARRAY_SIZE(data_b),     \
          data_c, ARRAY_SIZE(data_c))

const StatData case_1_in_a[2] = {
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
    {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}};
const StatData case_1_in_b[2] = {
    {.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode = 2},
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2}};
const StatData case_1_out[3] = {
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2},
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
    {.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2}};

const StatData case_2_in_a[3] = {
    {.id = 90067, .count = 1, .cost = 1.111, .primary = 0, .mode = 7},
    {.id = 90067, .count = 2, .cost = 2.222, .primary = 1, .mode = 6},
    {.id = 90067, .count = 3, .cost = 3.333, .primary = 0, .mode = 5}};
const StatData case_2_in_b[2] = {
    {.id = 90067, .count = 3, .cost = 3.333, .primary = 1, .mode = 4},
    {.id = 90189, .count = 100, .cost = 1.00003, .primary = 1, .mode = 2}};
const StatData case_2_out[2] = {
    {.id = 90189, .count = 100, .cost = 1.00003, .primary = 1, .mode = 2},
    {.id = 90067, .count = 9, .cost = 9.999, .primary = 0, .mode = 7}};

const StatData case_3_in_a[9] = {
    {.id = 90001, .count = 1, .cost = 6, .primary = 0, .mode = 1},
    {.id = 90001, .count = 2, .cost = 7, .primary = 1, .mode = 2},
    {.id = 90001, .count = 3, .cost = 8, .primary = 1, .mode = 3},
    {.id = 90001, .count = 4, .cost = 6, .primary = 0, .mode = 4},
    {.id = 90001, .count = 5, .cost = 7, .primary = 1, .mode = 5},
    {.id = 90001, .count = 6, .cost = 1, .primary = 0, .mode = 0},
    {.id = 90001, .count = 7, .cost = 2, .primary = 1, .mode = 1},
    {.id = 90001, .count = 8, .cost = 6, .primary = 1, .mode = 0},
    {.id = 90001, .count = 9, .cost = 1, .primary = 0, .mode = 1}};
const StatData case_3_in_b[1] = {
    {.id = 90001, .count = 10, .cost = 2, .primary = 1, .mode = 2}};
const StatData case_3_out[1] = {
    {.id = 90001, .count = 55, .cost = 46, .primary = 0, .mode = 5}};

const StatData case_4_in_a[1] = {
    {.id = 90067, .count = 1, .cost = 1, .primary = 1, .mode = 1}};
const StatData case_4_in_b[1] = {
    {.id = 90068, .count = 2, .cost = 2, .primary = 1, .mode = 2}};
const StatData case_4_out[2] = {
    {.id = 90067, .count = 1, .cost = 1, .primary = 1, .mode = 1},
    {.id = 90068, .count = 2, .cost = 2, .primary = 1, .mode = 2}};

const StatData case_5_in_a[1] = {
    {.id = 90067, .count = 1, .cost = 10, .primary = 1, .mode = 1}};
const StatData case_5_in_b[1] = {
    {.id = 90068, .count = 2, .cost = 2, .primary = 1, .mode = 2}};
const StatData case_5_out[2] = {
    {.id = 90068, .count = 2, .cost = 2, .primary = 1, .mode = 2},
    {.id = 90067, .count = 1, .cost = 10, .primary = 1, .mode = 1}};

const StatData case_6_in_a[6] = {
    {.id = 90067, .count = 1, .cost = 10, .primary = 1, .mode = 1},
    {.id = 90068, .count = 2, .cost = 9, .primary = 1, .mode = 3},
    {.id = 90069, .count = 1, .cost = 8, .primary = 1, .mode = 2},
    {.id = 90070, .count = 2, .cost = 7, .primary = 1, .mode = 1},
    {.id = 90071, .count = 1, .cost = 6, .primary = 1, .mode = 3},
    {.id = 90072, .count = 2, .cost = 5, .primary = 1, .mode = 1}};
const StatData case_6_in_b[1] = {
    {.id = 90072, .count = 1, .cost = 10, .primary = 0, .mode = 4}};
const StatData case_6_out[6] = {
    {.id = 90071, .count = 1, .cost = 6, .primary = 1, .mode = 3},
    {.id = 90070, .count = 2, .cost = 7, .primary = 1, .mode = 1},
    {.id = 90069, .count = 1, .cost = 8, .primary = 1, .mode = 2},
    {.id = 90068, .count = 2, .cost = 9, .primary = 1, .mode = 3},
    {.id = 90067, .count = 1, .cost = 10, .primary = 1, .mode = 1},
    {.id = 90072, .count = 3, .cost = 15, .primary = 0, .mode = 4}};

StatData case_7_in_a[MAX_BUFFER_SIZE];
StatData case_7_in_b[MAX_BUFFER_SIZE];
StatData case_7_out[MAX_BUFFER_SIZE];

const StatData case_8_in_a[2] = {
    {.id = 90073, .count = 1, .cost = 7, .primary = 1, .mode = 3},
    {.id = 90071, .count = 2, .cost = 6, .primary = 0, .mode = 4}};
const StatData *case_8_in_b = NULL;
const StatData case_8_out[2] = {
    {.id = 90071, .count = 2, .cost = 6, .primary = 0, .mode = 4},
    {.id = 90073, .count = 1, .cost = 7, .primary = 1, .mode = 3}};

const StatData case_9_in_a[2] = {
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
    {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}};
const StatData case_9_in_b[2] = {
    {.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode = 2},
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2}};
const StatData case_9_out[2] = {
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2},
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3}};

const StatData case_10_in_a[2] = {
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
    {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}};
const StatData case_10_in_b[2] = {
    {.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode = 2},
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2}};
const StatData case_10_out[3] = {
    {.id = 90189, .count = 0, .cost = 0, .primary = 0, .mode = 2},
    {.id = 90889, .count = 0, .cost = 0, .primary = 0, .mode = 3},
    {.id = 90089, .count = 0, .cost = 0, .primary = 0, .mode = 2}};

int RunProcess(char *path_in_1, char *path_in_2, char *path_out) {
  int pid = fork();

  if (pid == 0) {
    execl("./process", "process", path_in_1, path_in_2, path_out, NULL);
    perror("process execution failed");
    exit(1);
  } else if (pid > 0) {
    if (wait(NULL) == -1) {
      perror("wait failed");
      return -1;
    }
  } else {
    perror("fork failed.\n");
    return -1;
  }

  return 0;
}

/* 0 on success, 1 otherwise */
int RunTest(int test_id, const StatData *a_in, size_t a_size,
            const StatData *b_in, size_t b_size, const StatData *res,
            size_t res_size) {
  struct timeval start, end;
  gettimeofday(&start, NULL);

  StatData *out = NULL;
  size_t out_size = 0;

  char path_a[64];
  char path_b[64];
  char path_out[64];

  sprintf(path_a, "test_data/test_input_a_%d", test_id);
  sprintf(path_b, "test_data/test_input_b_%d", test_id);
  sprintf(path_out, "test_data/test_output_%d", test_id);

  if (StoreDump(a_in, a_size, path_a) != 0) {
    fprintf(stderr, "Test #%d: StoreDump failed on first input.\n", test_id);
    goto ERROR;
  }

  if (StoreDump(b_in, b_size, path_b) != 0) {
    fprintf(stderr, "Test #%d: StoreDump failed on second input.\n", test_id);
    goto ERROR;
  }

  int status = RunProcess(path_a, path_b, path_out);
  if (status == 0)
    out = LoadDump(path_out, &out_size);

  if (!out) {
    fprintf(stderr, "LoadDump() failed.\n");
    goto ERROR;
  }

  if (out_size != res_size) {
    fprintf(stderr, "Test #%d: wrong number of instances\n", test_id);
    goto ERROR;
  }

  for (size_t i = 0; i < out_size; i++) {
    int result = CompareData(out[i], res[i]);
    if (result != 0) {
      fprintf(stderr, "Test #%d: field %d doesn't match\n", test_id, result);
      goto ERROR;
    }
  }

  free(out);

  gettimeofday(&end, NULL);
  double elapsed =
      (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

  printf("Test #%d passed in %f seconds\n", test_id, elapsed);
  return 0;

ERROR:
  if (out)
    free(out);
  return 1;
}

void CreateMaxTest(StatData *data, size_t data_size, int is_out) {
  for (size_t i = 0; i < data_size; i++) {
    data[i].id = i + 1e9;
    data[i].count = is_out ? 2 * i + 2 : i + 1;
    data[i].cost = is_out ? 2.0 : 1.0;
    data[i].primary = 0;
    data[i].mode = (i % 8);
  }
}

int main() {
  srand(time(NULL));
  COMPLETE_TEST(1, case_1_in_a, case_1_in_b, case_1_out);
  COMPLETE_TEST(2, case_2_in_a, case_2_in_b, case_2_out);
  COMPLETE_TEST(3, case_3_in_a, case_3_in_b, case_3_out);
  COMPLETE_TEST(4, case_4_in_a, case_4_in_b, case_4_out);
  COMPLETE_TEST(5, case_5_in_a, case_5_in_b, case_5_out);
  COMPLETE_TEST(6, case_6_in_a, case_6_in_b, case_6_out);

  CreateMaxTest(case_7_in_a, MAX_BUFFER_SIZE, 0);
  CreateMaxTest(case_7_in_b, MAX_BUFFER_SIZE, 0);
  CreateMaxTest(case_7_out, MAX_BUFFER_SIZE, 1);
  COMPLETE_TEST(7, case_7_in_a, case_7_in_b, case_7_out);

  RunTest(8, case_8_in_a, 2, case_8_in_b, 0, case_8_out, 2);

  COMPLETE_TEST(9, case_9_in_a, case_9_in_b, case_9_out);
  COMPLETE_TEST(10, case_10_in_a, case_10_in_b, case_10_out);
  return 0;
}