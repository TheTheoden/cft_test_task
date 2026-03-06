#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

#define COMPLETE_TEST(test_id, data_a, data_b, data_c)                         \
  RunTest(test_id, data_a, NELEMS(data_a), data_b, NELEMS(data_b), data_c,     \
          NELEMS(data_c))

/* Содержимое для исходных файлов */
const StatData case_1_in_a[2] = {
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
    {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}};
const StatData case_1_in_b[2] = {
    {.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode = 2},
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2}};
/* Ожидаемый результат обработки */
const StatData case_1_out[3] = {
    {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2},
    {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
    {.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2}};

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

int RunTest(int test_id, const StatData *a_in, int a_size, const StatData *b_in,
            int b_size, const StatData *res, int res_size) {

  StatData *out = NULL;
  int out_size = 0;

  char path_a[64];
  char path_b[64];
  char path_out[64];

  sprintf(path_a, "test_data/test_input_a_%d", test_id);
  sprintf(path_b, "test_data/test_input_b_%d", test_id);
  sprintf(path_out, "test_data/test_output_%d", test_id);

  if (StoreDump(a_in, a_size, path_a) != 0) {
    fprintf(stderr, "StoreDump failed on first input.\n");
    goto ERROR;
  }

  if (StoreDump(b_in, b_size, path_b) != 0) {
    fprintf(stderr, "StoreDump failed on second input.\n");
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
    fprintf(stderr, "wrong number of instances\n");
    goto ERROR;
  }

  for (int i = 0; i < out_size; i++) {
    if (!CompareData(out[i], res[i])) {
      fprintf(stderr, "data doesn't match\n");
      goto ERROR;
    }
  }

  free(out);
  printf("test passed\n");
  return 0;

ERROR:
  if (out)
    free(out);
  return 1;
}

int main() {
  srand(time(NULL));
  COMPLETE_TEST(1, case_1_in_a, case_1_in_b, case_1_out);

  return 0;
}