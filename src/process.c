#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "3 arguments required\n");
    return 1;
  }

  StatData *a_in = NULL;
  StatData *b_in = NULL;
  StatData *out = NULL;
  int a_size = 0;
  int b_size = 0;
  int out_size = 0;

  char *path_a_in = argv[1];
  char *path_b_in = argv[2];
  char *path_out = argv[3];

  a_in = LoadDump(path_a_in, &a_size);

  if (!a_in) {
    fprintf(stderr, "can't load %s\n", path_a_in);
    goto ERROR;
  }

  b_in = LoadDump(path_b_in, &b_size);

  if (!b_in) {
    fprintf(stderr, "can't load %s\n", path_b_in);
    goto ERROR;
  }

  out = JoinDump(a_in, a_size, b_in, b_size, &out_size);

  if (!out) {
    fprintf(stderr, "JoinDump() failed\n");
    goto ERROR;
  }

  SortDump(out, out_size);

  for (int i = 0; i < 10 && i < out_size; i++) {
    PrintData(out[i]);
  }

  int status = StoreDump(out, out_size, path_out);
  if (status != 0) {
    fprintf(stderr, "StoreDump() failed\n");
    goto ERROR;
  }

  free(a_in);
  free(b_in);
  free(out);
  return 0;

ERROR:
  if (a_in)
    free(a_in);
  if (b_in)
    free(b_in);
  if (out)
    free(out);
  return 1;
}